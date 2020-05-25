#ifndef TAGLISTKEYEDIT_H
#define TAGLISTKEYEDIT_H

#include <QListWidget>
#include <QObject>
#include <QWidget>

#include "settingssingleton.h"
#include "tagkeyedit.h"
#include "taglistbase.h"

class TagKeyEditList : public TagListBase
{
public:
    explicit TagKeyEditList(QString tag_type, QString file_name = "", QWidget *parent = nullptr);

public:
    void save() override;
    void load() override;

private:
    QString tag_type; // common_extra or unique_extra
};

#endif // TAGLISTKEYEDIT_H
