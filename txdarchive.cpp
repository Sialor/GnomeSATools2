#include "txdarchive.h"

#include "imgarchiver.h"

#include <QDataStream>

#include <string.h>

extern const unsigned int g_rw_version;
extern const unsigned int g_size_of_sector;

extern ImgArchiver* g_imgArchiver;

TxdFile::TxdFile()
{
}

TxdFile::~TxdFile()
{
}

TxdInfo::TxdInfo()
{
}

TxdInfo::~TxdInfo()
{
}

Mipmap::Mipmap()
{
}

Mipmap::~Mipmap()
{
    if (m_data != nullptr)
    {
        delete[] m_data;

        m_data = nullptr;
    }
}

TxdTextureData::TxdTextureData()
    : m_palette(nullptr), m_dataSize(0), m_data(nullptr)
{
}

TxdTextureData::~TxdTextureData()
{
    if (m_data != nullptr)
    {
        delete[] m_data;

        m_data = nullptr;
    }

    if (m_palette != nullptr)
    {
        delete[] m_palette;

        m_palette = nullptr;
    }

    for (Mipmap* mip : m_mipmaps)
    {
        if (mip != nullptr)
        {
            delete mip;

            mip = nullptr;
        }
    }

    m_mipmaps.clear();
}

TxdExtraInfo::TxdExtraInfo()
    : m_chunkSize(0), m_data(nullptr)
{
}

TxdExtraInfo::~TxdExtraInfo()
{
    if (m_data != nullptr)
    {
        delete[] m_data;

        m_data = nullptr;
    }
}

TxdTexture::TxdTexture()
{
}

TxdTexture::~TxdTexture()
{
}

TxdArchive::TxdArchive()
{}

TxdArchive::~TxdArchive()
{
    for (TxdTexture* txt : m_textures)
    {
        if (txt != nullptr)
        {
            delete txt;

            txt = nullptr;
        }
    }

    m_textures.clear();
}

void TxdArchive::import_texture(QString fileName)
{
    QImage img(fileName, "PNG");

    QString name = fileName.split('/').last().split('.').first();

//    m_file.m_chunkSize;


//    m_info.m_chunkSize;
    m_info.m_count++;


    m_textures.append(new TxdTexture());
    m_textures.last()->m_id = 0x15;
//    m_textures.last()->m_chunkSize;


    TxdTextureData &tdata = m_textures.last()->m_textureData;

    tdata.m_id = 0x01;
//    tdata.m_chunkSize = ;

    tdata.m_version = 0x09;
    tdata.m_filterFlags = 0x1101;

    name = name.left(31);

    for (int i(0); i < name.length(); ++i)
    {
        tdata.m_textureName[i] = name.toStdString().c_str()[i];
        tdata.m_alphaName[i] = tdata.m_textureName[i];
    }

    tdata.m_textureName[name.length()] = '\0';
    tdata.m_alphaName[name.length()] = '\0';

    tdata.m_alphaFlags = 0x0500;
    tdata.m_direct3dTextureFormat = 0x15;
    tdata.m_width = img.width();
    tdata.m_height = img.height();

    tdata.m_depth = 0x20;
    tdata.m_mipmapCount = 0x01;
    tdata.m_texcodeType = 0x04;
    tdata.m_flags = 0x01;

    if (tdata.m_depth == 8)
    {
        tdata.m_palette = new char[256 * 4];
    }

    tdata.m_dataSize = img.sizeInBytes();
    tdata.m_data = new char[tdata.m_dataSize];

    for (int y = 0; y < img.height(); y++)
    {
        memcpy(tdata.m_data + y * img.width() * 4, img.scanLine(y), img.bytesPerLine());
    }


    for (int i(1); i < tdata.m_mipmapCount; ++i)
    {
        tdata.m_mipmaps.append(new Mipmap());

        // забивка mipmap
    }



    tdata.m_chunkSize = 4 * 5 + 32 + 32 + 4 * 2 + 2 * 2 + 1 * 4;

    tdata.m_chunkSize += tdata.m_depth == 8 ? 256 * 4 : 0;
    tdata.m_chunkSize += 4 * 1 + tdata.m_dataSize;

    for (int i(1); i < tdata.m_mipmapCount; ++i)
    {
        tdata.m_chunkSize += tdata.m_mipmaps.at(i)->m_dataSize;
    }



    m_textures.last()->m_chunkSize = tdata.m_chunkSize + 4 * 3;



    m_info.m_chunkSize += m_textures.last()->m_chunkSize;



    m_file.m_chunkSize += m_textures.last()->m_chunkSize;
}

void TxdArchive::delete_texture(QString name)
{
    unsigned int index;

    for (index = 0; index < m_textures.count(); ++index)
    {
        if (m_textures.at(index)->m_textureData.m_textureName == name)
        {
            break;
        }
    }

    unsigned int size = m_textures.at(index)->m_chunkSize;
    m_file.m_chunkSize -= size;
    m_info.m_chunkSize -= size;
    m_info.m_count--;

    if (m_textures.at(index) != nullptr)
    {
        delete m_textures.at(index);
    }

    m_textures.removeAt(index);
}





QDataStream& operator>>(QDataStream& in, TxdArchive& arch)
{
    in >> arch.m_file
       >> arch.m_info;

    for (int j(0); j < arch.m_info.m_count; ++j)
    {
        arch.m_textures.append(new TxdTexture());

        in >> *arch.m_textures.last();
    }

    in >> arch.m_extraInfo;

    return in;
}

QDataStream& operator>>(QDataStream& in, TxdFile& file)
{
    in >> file.m_id
       >> file.m_chunkSize;

    // rw version
    in.skipRawData(4);

    return in;
}

QDataStream& operator>>(QDataStream& in, TxdInfo& info)
{
    in >> info.m_id
       >> info.m_chunkSize;

    // rw version
    in.skipRawData(4);

    return in >> info.m_count
              >> info.m_unknown;
}

QDataStream& operator>>(QDataStream& in, TxdTexture& texture)
{
    in >> texture.m_id
       >> texture.m_chunkSize;

    // rw version
    in.skipRawData(4);

    return in >> texture.m_textureData
              >> texture.m_extraInfo;
}

QDataStream& operator>>(QDataStream& in, TxdTextureData& textureData)
{
    in >> textureData.m_id
       >> textureData.m_chunkSize;

    // rw version
    in.skipRawData(4);

    in >> textureData.m_version
       >> textureData.m_filterFlags;

    in.readRawData(textureData.m_textureName, 32);
    in.readRawData(textureData.m_alphaName, 32);

    // replace garbage in c-style string to 0
    bool t(false), a(false);
    for (unsigned int i(0); i < 32; ++i)
    {
        if (textureData.m_textureName[i] == '\0' &&
                !t)
        {
            t = true;
        }

        if (textureData.m_alphaName[i] == '\0' &&
                !a)
        {
            a = true;
        }

        if (t)
        {
            textureData.m_textureName[i] = '\0';
        }

        if (a)
        {
            textureData.m_alphaName[i] = '\0';
        }
    }

    in >> textureData.m_alphaFlags
       >> textureData.m_direct3dTextureFormat
       >> textureData.m_width
       >> textureData.m_height
       >> (quint8&)textureData.m_depth
       >> (quint8&)textureData.m_mipmapCount
       >> (quint8&)textureData.m_texcodeType
       >> (quint8&)textureData.m_flags;

    // !!! изучить возможные варианты depth
    if (textureData.m_depth == 8)
    {
        textureData.m_palette = new char[256 * 4];

        in.readRawData(textureData.m_palette, 256 * 4);
    }

    in >> textureData.m_dataSize;

    textureData.m_data = new char[textureData.m_dataSize];

    in.readRawData(textureData.m_data, textureData.m_dataSize);

    for (int i(0); i < textureData.m_mipmapCount - 1; ++i)
    {
        textureData.m_mipmaps.append(new Mipmap());

        in >> *textureData.m_mipmaps.last();
    }

    return in;
}

QDataStream& operator>>(QDataStream& in, TxdExtraInfo& info)
{
    in >> info.m_id
       >> info.m_chunkSize;

    // rw version
    in.skipRawData(4);

    if (info.m_chunkSize > 0)
    {
        info.m_data = new char[info.m_chunkSize];

        in.readRawData(info.m_data, info.m_chunkSize);
    }

    return in;
}

QDataStream& operator>>(QDataStream& in, Mipmap& mipmap)
{
    in >> mipmap.m_dataSize;

    if (mipmap.m_dataSize > 0)
    {
        mipmap.m_data = new char[mipmap.m_dataSize];

        in.readRawData(mipmap.m_data, mipmap.m_dataSize);
    }

    return in;
}










QDataStream& operator<<(QDataStream& out, TxdArchive& arch)
{
    out << arch.m_file
        << arch.m_info;

    for (auto texture : arch.m_textures)
    {
        out << *texture;
    }

    out << arch.m_extraInfo;

    return out;
}

QDataStream& operator<<(QDataStream& out, TxdFile& file)
{
    return out << file.m_id
               << file.m_chunkSize
               << g_rw_version;
}

QDataStream& operator<<(QDataStream& out, TxdInfo& info)
{
    return out << info.m_id
               << info.m_chunkSize
               << g_rw_version
               << info.m_count
               << info.m_unknown;
}

QDataStream& operator<<(QDataStream& out, TxdTexture& texture)
{
    out << texture.m_id
        << texture.m_chunkSize
        << g_rw_version;

    out << texture.m_textureData;

    out << texture.m_extraInfo;

    return out;
}

QDataStream& operator<<(QDataStream& out, TxdTextureData& textureData)
{
    out << textureData.m_id
        << textureData.m_chunkSize
        << g_rw_version
        << textureData.m_version
        << textureData.m_filterFlags;

    writeString(out, QString(textureData.m_textureName), 32);
    writeString(out, QString(textureData.m_alphaName), 32);

    out << textureData.m_alphaFlags
        << textureData.m_direct3dTextureFormat
        << textureData.m_width
        << textureData.m_height
        << (quint8&)textureData.m_depth
        << (quint8&)textureData.m_mipmapCount
        << (quint8&)textureData.m_texcodeType
        << (quint8&)textureData.m_flags;

    /// !!! изучить этот момент. depth может иметь другое значение
    if (textureData.m_depth == 8)
    {
        out.writeRawData(textureData.m_palette, 256 * 4);
    }

    out << textureData.m_dataSize;

    out.writeRawData(textureData.m_data, textureData.m_dataSize);

    for (auto mipmap : textureData.m_mipmaps)
    {
        out << *mipmap;
    }

    return out;
}

QDataStream& operator<<(QDataStream& out, TxdExtraInfo& info)
{
    out << info.m_id
        << info.m_chunkSize
        << g_rw_version;

    if (info.m_chunkSize > 0)
    {
        out.writeRawData(info.m_data, info.m_chunkSize);
    }

    return out;
}

QDataStream& operator<<(QDataStream& out, Mipmap& mipmap)
{
    out << mipmap.m_dataSize;

    if (mipmap.m_dataSize > 0)
    {
        out.writeRawData(mipmap.m_data, mipmap.m_dataSize);
    }

    return out;
}
