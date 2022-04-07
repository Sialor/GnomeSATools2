#include "dffarchive.hpp"

extern const unsigned int g_rw_version;

DffArchive::DffArchive()
    : m_data(nullptr)
{

}

DffArchive::~DffArchive()
{
    if (m_data != nullptr)
    {
        delete[] m_data;

        m_data = nullptr;
    }
}

QDataStream& operator>>(QDataStream& in, DffArchive& dff)
{
    in >> dff.m_id
       >> dff.m_chunkSize;

    // rw version
    in.skipRawData(4);

    dff.m_data = new char[dff.m_chunkSize];

    in.readRawData(dff.m_data, dff.m_chunkSize);

    return in;
}

QDataStream& operator<<(QDataStream& out, DffArchive& dff)
{
    out << dff.m_id
        << dff.m_chunkSize
        << g_rw_version;

    out.writeRawData(dff.m_data, dff.m_chunkSize);

    return out;
}
