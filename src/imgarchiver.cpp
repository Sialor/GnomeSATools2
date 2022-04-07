#include "imgarchiver.hpp"

#include <QFile>
#include <QFileInfo>
#include <QString>

#include "mainwindow.hpp"

//#define REALIZE_ADVANCE_READING_DFF

extern const unsigned int g_size_of_sector;

extern ImgArchiver* g_imgArchiver;

extern MainWindow* g_mainWindow;

/// bytes to blocks
inline unsigned int by2bl(unsigned int size)
{
    return size / g_size_of_sector + (size % g_size_of_sector == 0 ? 0 : 1);
}

Dir::Dir()
    : m_offset(0), m_streamingSize(0), m_sizeArchive(0), m_name("")
{
}

Dir::~Dir()
{
}

ImgArchiver::ImgArchiver()
{
}

ImgArchiver::~ImgArchiver()
{
    forget();
}

void ImgArchiver::read()
{
    QFile file(m_fileName);

    file.open(QIODevice::ReadOnly);

    if (!file.exists())
    {
        // вывод сообщения что файл не существует
    }

    QByteArray bytes(file.readAll());

    file.close();

    QDataStream in(bytes);
    in.setByteOrder(QDataStream::LittleEndian);

    in >> m_header;

    if (m_header.m_version != "VER2")
    {
        // вывод сообщения о не поддерживаемой версии архива

        return;
    }

    qint64 backpoint;

    for (unsigned int i(0); i < m_header.m_numberOfEntries; ++i)
    {
        m_dirs.append(new Dir());

        in >> *m_dirs.last();

        backpoint = in.device()->pos();

        in.device()->seek(m_dirs.last()->m_offset * g_size_of_sector);

        unsigned int type;

        (in >> type)
                .device()->seek(in.device()->pos() - 4);

        if (type == 0x10)
        {
            m_dffs.append(new DffArchive());

#ifdef REALIZE_ADVANCE_READING_DFF
            in >> *m_dffs.last();
#else
            m_dffs.at(i)->m_chunkSize = m_dirs.at(i)->m_sizeArchive * g_size_of_sector;
            m_dffs.at(i)->m_data = new char[m_dffs.at(i)->m_chunkSize];
            in.readRawData(m_dffs.at(i)->m_data, m_dffs.at(i)->m_chunkSize);
#endif
        }
        else if (type == 0x16)
        {
            m_txds.append(new TxdArchive());

            in >> *m_txds.last();
        }

        in.device()->seek(backpoint);
    }
}

void ImgArchiver::write()
{
    char *emptySector = new char[g_size_of_sector];

    // all bytes is 0
    for (unsigned int i(0); i < g_size_of_sector; ++i)
    {
        emptySector[i] = 0x00;
    }

    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::OpenModeFlag::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out << m_header;

    for (auto dir : m_dirs)
    {
        out << *dir;
    }

    // skip bytes
    unsigned int skipSize = by2bl(out.device()->pos()) * g_size_of_sector - out.device()->pos();

    out.writeRawData(emptySector, skipSize);

    for (int i(0); i < m_dffs.count(); ++i)
    {
#ifdef REALIZE_ADVANCE_READING_DFF
        out << *m_dffs.at(i);
#else
        out.writeRawData(m_dffs.at(i)->m_data, m_dffs.at(i)->m_chunkSize);
#endif
//        skipSize = m_dirs.at(i)->m_sizeArchive * g_size_of_sector;
//        skipSize -= m_dffs.at(i)->m_chunkSize + 12;

//        out.writeRawData(emptySector, skipSize);
    }

    for (int i(0); i < m_txds.count(); ++i)
    {
        out << *m_txds.at(i);

        skipSize = m_dirs.at(m_dffs.count() + i)->m_sizeArchive * g_size_of_sector;
        skipSize -= m_txds.at(i)->m_file.m_chunkSize + 12;

        out.writeRawData(emptySector, skipSize);
    }

    if (emptySector != nullptr)
    {
        delete[] emptySector;

        emptySector = nullptr;
    }

    QFile file(m_fileName);
    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

void ImgArchiver::forget()
{
//    m_header.m_version = "";

    for (auto dir : m_dirs)
    {
        if (dir != nullptr)
        {
            delete dir;

            dir = nullptr;
        }
    }

    for (auto dff : m_dffs)
    {
        if (dff != nullptr)
        {
            delete dff;

            dff = nullptr;
        }
    }

    for (auto txd : m_txds)
    {
        if (txd != nullptr)
        {
            delete txd;

            txd = nullptr;
        }
    }

    m_dirs.clear();
    m_dffs.clear();
    m_txds.clear();
}

void ImgArchiver::set_filename(QString fileName)
{
    m_fileName = fileName;

    if (g_mainWindow->m_item != nullptr)
    {
        g_mainWindow->m_item->setText(0, fileName);
    }
}

QString ImgArchiver::get_filename()
{
    return m_fileName;
}

/// return true if found
bool ImgArchiver::get_index(QString name, unsigned int &index)
{
    for (index = 0; index < m_dirs.count(); ++index)
    {
        if (m_dirs[index]->m_name == name)
        {
            return true;
        }
    }

    return false;
}

/// return true if deleted
bool ImgArchiver::delete_file(QString name)
{
    unsigned int index;

    if (!get_index(name, index))
    {
        // file not found

        return false;
    }

    if (m_dirs[index] != nullptr)
    {
        delete m_dirs[index];

        m_dirs[index] = nullptr;
    }

    m_dirs.removeAt(index);

    if (name.right(3) == "dff")
    {
        if (m_dffs[index] != nullptr)
        {
            delete m_dffs[index];

            m_dffs[index] = nullptr;
        }

        m_dffs.removeAt(index);
    }
    else if (name.right(3) == "txd")
    {
        if (m_txds[index - m_dffs.count()] != nullptr)
        {
            delete m_txds[index];

            m_txds[index] = nullptr;
        }

        m_txds.removeAt(index);
    }

    return true;
}

bool ImgArchiver::is_empty()
{
    return m_dirs.count() == 0;
}

void ImgArchiver::unpack(QString fileName, QUrl path)
{
    unsigned int index(0);
    if (!g_imgArchiver->get_index(fileName, index))
    {
        // предупреждение что файл не найден в архиве

        return;
    }

    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::OpenModeFlag::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    if (fileName.right(3) == "dff")
    {
        out << *m_dffs.at(index);
    }
    else // txd
    {
        out << *m_txds.at(index - m_dffs.count());
    }

    QFile file(path.toString() + '/' + fileName);
    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

QDataStream& writeString(QDataStream& out, const QString& str, unsigned int sizeField)
{
    char *emptyStr = new char[sizeField];

    for (unsigned int i(0); i < sizeField; ++i)
    {
        emptyStr[i] = 0;
    }

    auto strbytes = str.toLatin1();
    int numbers = out.writeRawData(strbytes.data(), strbytes.count() >= sizeField ? sizeField - 1 : strbytes.count());

    out.writeRawData(emptyStr, sizeField - numbers);

    if (emptyStr != nullptr)
    {
        delete[] emptyStr;

        emptyStr = nullptr;
    }

    return out;
}


QDataStream& operator>>(QDataStream& in, ImgHeader& header)
{
    char ver[5];

    in.readRawData(ver, 4);

    ver[4] = '\0';

    header.m_version = QString(ver);

    return in >> header.m_numberOfEntries;
}

QDataStream& operator>>(QDataStream& in, Dir& dir)
{
    in >> dir.m_offset
       >> dir.m_sizeArchive
       >> dir.m_streamingSize;

    in.readRawData(dir.m_name, 24);

    return in;
}

QDataStream& operator<<(QDataStream& out, ImgHeader& header)
{
    out.writeRawData(header.m_version.toLatin1().data(), 4);
    return out << header.m_numberOfEntries;
}

QDataStream& operator<<(QDataStream& out, Dir& dir)
{
    out << dir.m_offset << dir.m_sizeArchive << (unsigned short)0x00;

    out.writeRawData(dir.m_name, 24);

    return out;
}
