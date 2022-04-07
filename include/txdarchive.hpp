#ifndef TXDARCHIVE_H
#define TXDARCHIVE_H

#include <QDataStream>
#include <QVector>

class TxdFile
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    TxdFile();

    ~TxdFile();
};

class TxdInfo
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    unsigned short m_count;
    unsigned short m_unknown;

    TxdInfo();

    ~TxdInfo();
};

class Mipmap
{
public:
    unsigned int m_dataSize;
    char* m_data;

    Mipmap();

    ~Mipmap();
};

class TxdTextureData
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    unsigned int m_version;
    unsigned int m_filterFlags;

    char  m_textureName[32];
    char  m_alphaName[32];

    unsigned int m_alphaFlags;
    unsigned int m_direct3dTextureFormat;
    unsigned short m_width;
    unsigned short m_height;

    char  m_depth;
    char  m_mipmapCount;
    char  m_texcodeType;
    char  m_flags;

    char* m_palette; // [m_depth == 8 ? 256 * 4 : 0]

    unsigned int m_dataSize;

    char* m_data; // m_dataSize

    QVector<Mipmap*> m_mipmaps; // [m_mipmapCount - 1]

    TxdTextureData();

    ~TxdTextureData();
};

class TxdExtraInfo
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    char* m_data;

    TxdExtraInfo();

    ~TxdExtraInfo();
};

class TxdTexture
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    TxdTextureData m_textureData;
    TxdExtraInfo m_extraInfo;

    TxdTexture();

    ~TxdTexture();
};

class TxdArchive
{
public:
    TxdFile m_file;
    TxdInfo m_info;
    QVector<TxdTexture*> m_textures;
    TxdExtraInfo m_extraInfo;

    TxdArchive();
    ~TxdArchive();

    void import_texture(QString fileName);

    void delete_texture(QString name);
};

QDataStream& operator>>(QDataStream& in, TxdArchive& arch);

QDataStream& operator>>(QDataStream& in, TxdFile& file);

QDataStream& operator>>(QDataStream& in, TxdInfo& info);

QDataStream& operator>>(QDataStream& in, TxdTexture& texture);

QDataStream& operator>>(QDataStream& in, TxdTextureData& textureData);

QDataStream& operator>>(QDataStream& in, TxdExtraInfo& info);

QDataStream& operator>>(QDataStream& in, Mipmap& mipmap);


QDataStream& operator<<(QDataStream& out, TxdArchive& arch);

QDataStream& operator<<(QDataStream& out, TxdFile& file);

QDataStream& operator<<(QDataStream& out, TxdInfo& info);

QDataStream& operator<<(QDataStream& out, TxdTexture& texture);

QDataStream& operator<<(QDataStream& out, TxdTextureData& textureData);

QDataStream& operator<<(QDataStream& out, TxdExtraInfo& info);

QDataStream& operator<<(QDataStream& out, Mipmap& mipmap);

#endif // TXDARCHIVE_H
