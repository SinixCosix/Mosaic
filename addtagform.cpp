#include "addtagform.h"
#include "ui_addtagform.h"

AddTagForm::AddTagForm(QWidget *parent) : QDialog(parent), ui(new Ui::AddTagForm)
{
    ui->setupUi(this);

    //    ui->testBtn->setVisible(false);
    loadTags();

    //    list = new TagListWidget(this);

    TagKeyEditWidget *t = new TagKeyEditWidget();
    connect(t, &TagKeyEditWidget::checkValid, this, &AddTagForm::isValid);
}

AddTagForm::~AddTagForm()
{
    delete ui;
}

void AddTagForm::addTag()
{
    TagKeyEditWidget *tag = new TagKeyEditWidget(this);
    connect(tag, &TagKeyEditWidget::checkValid, this, &AddTagForm::isValid);
    connect(tag, SIGNAL(removeTag(QPoint)), this, SLOT(removeTag(QPoint)));
    QListWidgetItem *item = new QListWidgetItem(ui->tagList);
    item->setSizeHint(tag->sizeHint());
    ui->tagList->setItemWidget(item, tag);
    ui->tagList->addItem(item);
}

void AddTagForm::addTag(TagKeyEditWidget *tag)
{
    connect(tag, &TagKeyEditWidget::checkValid, this, &AddTagForm::isValid);
    connect(tag, SIGNAL(removeTag(QPoint)), this, SLOT(removeTag(QPoint)));
    QListWidgetItem *item = new QListWidgetItem(ui->tagList);
    item->setSizeHint(tag->sizeHint());
    ui->tagList->setItemWidget(item, tag);
    ui->tagList->addItem(item);
}

void AddTagForm::removeTag(QPoint p)
{
    qDebug() << "addtag removetag" << p;
    auto current_tag = ui->tagList->itemAt(p);
    auto row         = ui->tagList->row(current_tag);
    delete ui->tagList->takeItem(row);
}

void AddTagForm::saveTags()
{
    qDebug() << "saveTags";
    QList<QHash<QString, QString>> extra_exif;
    for (int i = 0; i < ui->tagList->count(); ++i)
    {
        QListWidgetItem *item    = ui->tagList->item(i);
        QWidget *tag_wgt         = ui->tagList->itemWidget(item);
        QObjectList tag_sub_wgts = tag_wgt->children();
        QLineEdit *ldt           = qobject_cast<QLineEdit *>(tag_sub_wgts[1]);
        QComboBox *cbx           = qobject_cast<QComboBox *>(tag_sub_wgts[2]);

        extra_exif.append(QHash<QString, QString> {{ldt->text(), cbx->currentText()}});
    }
    SettingsSingleton::getInstance().setExtraExif(extra_exif);
}

void AddTagForm::loadTags()
{
    qDebug() << "loadTags";
    QList<QHash<QString, QString>> tags = SettingsSingleton::getInstance().getExtraExif();
    for (auto item : tags)
    {
        TagKeyEditWidget *tag = new TagKeyEditWidget(this);
        tag->setShortName(item.begin().key());
        tag->setExifName(item.begin().value());
        addTag(tag);
    }
}

void AddTagForm::chooseKeyAction(QString key)
{
    if (key == "\u000E") // ctrl + n
        addTag();
    else if (key == "\u007F") // delete
        removeTag();
}

void AddTagForm::removeTag()
{
    if (!ui->tagList->selectedItems().isEmpty())
    {
        auto item = ui->tagList->selectedItems().first();
        int row   = ui->tagList->row(item);
        delete ui->tagList->takeItem(row);
    }
}

bool AddTagForm::isValid()
{
    bool isValid = true;
    for (int i = 0; i < ui->tagList->count(); ++i)
    {
        QListWidgetItem *item = ui->tagList->item(i);
        QWidget *tag_wgt      = ui->tagList->itemWidget(item);
        TagKeyEditWidget *t   = dynamic_cast<TagKeyEditWidget *>(tag_wgt);

        QObjectList tag_sub_wgts = tag_wgt->children();
        QLineEdit *ldt           = qobject_cast<QLineEdit *>(tag_sub_wgts[1]);
        QComboBox *cbx           = qobject_cast<QComboBox *>(tag_sub_wgts[2]);
        if (cbx->findText(cbx->currentText()) == -1 or ldt->text().isEmpty())
        {
            isValid = false;
            QColor color;
            color.setRgb(255, 102, 102);
            item->setBackground(color);
        }
        else
            item->setBackground(Qt::white);
    }
    return isValid;
}

void AddTagForm::on_buttonBox_accepted()
{
    if (isValid())
    {
        saveTags();
        close();
    }
}

void AddTagForm::on_addBtn_clicked()
{
    addTag();
}

void AddTagForm::on_testBtn_clicked()
{
    TagKeyEditWidget *t = new TagKeyEditWidget();
    //    TagValueEditWidget *t = new TagValueEditWidget();
    ui->list->addTag(t);
}

void AddTagForm::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
    qDebug() << event->text();
    chooseKeyAction(event->text());
}

void AddTagForm::accept()
{
    if (isValid())
    {
        saveTags();
        close();
    }
}

template <typename T>
void AddTagForm::addTagTest(T *tag)
{
    connect(tag, &T::checkValid, this, &AddTagForm::isValid);
    connect(tag, SIGNAL(removeTag(QPoint)), this, SLOT(removeTag(QPoint)));
    QListWidgetItem *item = new QListWidgetItem(ui->tagList);
    item->setSizeHint(tag->sizeHint());
    ui->tagList->setItemWidget(item, tag);
    ui->tagList->addItem(item);
}
