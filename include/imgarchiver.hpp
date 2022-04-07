#ifndef IMGARCHIVER_H
#define IMGARCHIVER_H

#include <QUrl>
#include <QString>
#include <QVector>
#include <QPixmap>

#include "txdarchive.hpp"
#include "dffarchive.hpp"

class Dir
{
public:
    unsigned int m_offset;         // sectors
    unsigned short m_streamingSize ; // ^
    unsigned short m_sizeArchive;    // ^

    char m_name[24];

    Dir();

    ~Dir();
};

class ImgHeader
{
public:
    QString m_version;

    unsigned int m_numberOfEntries;
};

class ImgArchiver
{
public:
    QString m_fileName;

    ImgHeader m_header;

    QVector<Dir*>        m_dirs;
    QVector<DffArchive*> m_dffs;
    QVector<TxdArchive*> m_txds;

    ImgArchiver();

    ~ImgArchiver();


    void read();

    void write();

    void forget();


    void set_filename(QString fileName);

    QString get_filename();


    bool delete_file(QString name);

    bool is_empty();

    void unpack(QString fileName, QUrl path);

    bool get_index(QString name, unsigned int &index);
};

QDataStream& writeString(QDataStream& out, const QString& str, unsigned int sizeField);

QDataStream& operator>>(QDataStream& in, ImgHeader& header);

QDataStream& operator>>(QDataStream& in, Dir& dir);

QDataStream& operator<<(QDataStream& out, ImgHeader& header);

QDataStream& operator<<(QDataStream& out, Dir& dir);

#endif // IMGARCHIVER_H
