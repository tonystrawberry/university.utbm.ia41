#ifndef Settings_h
#define Settings_h


#include <QtGui>

#include "Math/VectorNumber.h"


class Settings
{
    public:
        Settings();
        ~Settings();

        void init( const char* settings_name );

        void initParameters();


    protected:
        QVector<QString> m_SettingName;
        QVector<QStringList> m_SettingsList;
        VectorNumber* m_SettingId;
        int32 m_NbrSettings;
};


#endif
