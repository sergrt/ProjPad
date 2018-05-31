#include "stdafx.h"
#include "Settings.h"
#include <QSettings>

namespace SettingsFile {
    const QString fileName = "settings.ini";
    
    namespace Main {
        const QString sectionName = "Main";
        const QString fontOverride = "FontOverrideQss";
        const QString theme = "Theme";
    }
}

Settings::Settings() {
    load();
}
void Settings::load() {
    QSettings s(SettingsFile::fileName, QSettings::IniFormat);
    s.beginGroup(SettingsFile::Main::sectionName);

    setFontOverride(s.value(SettingsFile::Main::fontOverride).toString());
    setTheme(s.value(SettingsFile::Main::theme).toString());
}
Settings::~Settings() {
    save();
}
void Settings::save() const {
    QSettings s(SettingsFile::fileName, QSettings::IniFormat);
    s.beginGroup(SettingsFile::Main::sectionName);

    s.setValue(SettingsFile::Main::fontOverride, fontOverride());
    s.setValue(SettingsFile::Main::theme, theme());
}

QString Settings::fontOverride() const {
    return fontOverride_;
}
QString Settings::theme() const {
    return theme_;
}
void Settings::setFontOverride(const QString& qss) {
    fontOverride_ = qss;
    //save();
}
void Settings::setTheme(const QString& themeName) {
    theme_ = themeName;
    //save();
}