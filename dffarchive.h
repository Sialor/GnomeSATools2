#ifndef DFFARCHIVE_H
#define DFFARCHIVE_H

#include <QDataStream>

class DffArchive
{
public:
    unsigned int m_id;
    unsigned int m_chunkSize;

    char* m_data;

    DffArchive();

    ~DffArchive();
};

QDataStream& operator>>(QDataStream& in, DffArchive& dff);

QDataStream& operator<<(QDataStream& out, DffArchive& dff);

#endif // DFFARCHIVE_H
