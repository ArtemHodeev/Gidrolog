#include "checkboxdelegate.h"
#include <QCheckBox>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent ):QItemDelegate(parent)
{
}
void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawDisplay(painter,option,option.rect,index.model()->data( index, Qt::DisplayRole ).toBool()?QString("      ").append(tr("Отображать")):QString("      ").append(tr("Не отображать")));
    drawFocus(painter,option,option.rect);
}
QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    theCheckBox = new QCheckBox( parent );
    QObject::connect(theCheckBox,SIGNAL(toggled(bool)),this,SLOT(setData(bool)));
    return theCheckBox;
}
void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int val = index.model()->data( index, Qt::DisplayRole ).toInt();

    (static_cast<QCheckBox*>( editor ))->setChecked(val);
}
void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData( index, (int)(static_cast<QCheckBox*>( editor )->isChecked() ) );
}
void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry( option.rect );
}
void CheckBoxDelegate::setData(bool val)
{
    emit commitData(theCheckBox);
}
//static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options) {
//  QStyleOptionButton check_box_style_option;
//  QRect check_box_rect = QApplication::style()->subElementRect(
//      QStyle::SE_CheckBoxIndicator,
//      &check_box_style_option);
//  QPoint check_box_point(view_item_style_options.rect.x() +
//                         view_item_style_options.rect.width() / 2 -
//                         check_box_rect.width() / 2,
//                         view_item_style_options.rect.y() +
//                         view_item_style_options.rect.height() / 2 -
//                         check_box_rect.height() / 2);
//  return QRect(check_box_point, check_box_rect.size());
//}


//CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
//    QItemDelegate(parent)
//{
//}
//CheckBoxDelegate::~CheckBoxDelegate()
//{}
//QWidget* CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    QCheckBox *box = new QCheckBox(parent);
//    return box;
//}
//void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    bool value = index.model()->data(index).toBool();
//    QCheckBox* box = static_cast<QCheckBox*>(editor);
//    box->setChecked(value);
//}
//void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//    QCheckBox* box = static_cast<QCheckBox*>(editor);
//    bool value = box->isChecked();
//    model->setData(index,value);
//}
//void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    editor->setGeometry(option.rect);
//}
//void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
//    QStyleOptionButton check_box_style_option;
//    check_box_style_option.state |= QStyle::State_Enabled;
//    if (checked) {
//      check_box_style_option.state |= QStyle::State_On;
//    } else {
//      check_box_style_option.state |= QStyle::State_Off;
//    }
//    check_box_style_option.rect = CheckBoxRect(option);
//    QApplication::style()->drawControl(QStyle::CE_CheckBox,
//                                       &check_box_style_option,
//                                       painter);
//}
//void CheckBoxDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state)
//{

//}
//bool CheckBoxDelegate::editorEvent(QEvent *event,
//                                   QAbstractItemModel *model,
//                                   const QStyleOptionViewItem &option,
//                                   const QModelIndex &index)
//{
//    if ((event->type() == QEvent::MouseButtonRelease) ||
//                    (event->type() == QEvent::MouseButtonDblClick)) {
//        QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
//        if (mouse_event->button() != Qt::LeftButton ||
//                        !CheckBoxRect(option).contains(mouse_event->pos())) {
//            return false;
//        }
//        if (event->type() == QEvent::MouseButtonDblClick) {
//            return true;
//        }
//    } else if (event->type() == QEvent::KeyPress) {
//        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
//                        static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) {
//            return false;
//        }
//    } else {
//        return false;
//    }

//    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
//    return model->setData(index, !checked, Qt::EditRole);
//}
