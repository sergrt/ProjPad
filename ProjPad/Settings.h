#pragma once
#include <QString>

class Settings {
public:
    Settings();
    ~Settings();
    QString fontOverride() const;
    QString theme() const;
    bool restoreSession() const;

    void setFontOverride(const QString& qss);
    void setTheme(const QString& themeName);
    
    void save() const;
private:
    void load();
    QString fontOverride_;
    QString theme_;
    bool restoreSession_;
};
