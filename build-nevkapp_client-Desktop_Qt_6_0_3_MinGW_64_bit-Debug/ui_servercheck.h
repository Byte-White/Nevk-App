/********************************************************************************
** Form generated from reading UI file 'servercheck.ui'
**
** Created by: Qt User Interface Compiler version 6.0.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERCHECK_H
#define UI_SERVERCHECK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_ServerCheck
{
public:
    QLabel *label;

    void setupUi(QDialog *ServerCheck)
    {
        if (ServerCheck->objectName().isEmpty())
            ServerCheck->setObjectName(QString::fromUtf8("ServerCheck"));
        ServerCheck->setEnabled(true);
        ServerCheck->resize(500, 500);
        ServerCheck->setWindowOpacity(0.700000000000000);
        label = new QLabel(ServerCheck);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 140, 421, 201));
        QFont font;
        font.setPointSize(26);
        label->setFont(font);
        label->setCursor(QCursor(Qt::ArrowCursor));

        retranslateUi(ServerCheck);

        QMetaObject::connectSlotsByName(ServerCheck);
    } // setupUi

    void retranslateUi(QDialog *ServerCheck)
    {
        ServerCheck->setWindowTitle(QString());
        label->setText(QCoreApplication::translate("ServerCheck", "CHECKING SERVER...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerCheck: public Ui_ServerCheck {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERCHECK_H
