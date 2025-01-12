#include "info.h"
#include "ui_info.h"


#define test std::string("abcdef");

Info::Info(QWidget *parent) : QDialog(parent), ui(new Ui::Info)
{
    ui->setupUi(this);
    //.arg(_PROJECT_VERSION_)
    auto tbText = QString(
            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
            "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">"
            "p, li { white-space: pre-wrap; }"
            "hr { height: 1px; border-width: 0; }"
            "li.unchecked::marker { content: \"\2610\"; }"
            "li.checked::marker { content: \"\2612\"; }"
            "</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Built by VxGrid: <a href=\"https://github.com/VxGrid/\"><span style=\" text-decoration: underline; color:#007af4;\">https://github.com/VxGrid/</span></a></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Using</span></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">E57 (<a href=\"https://github.com/asmaloney/libE57Format?tab=BSL-1.0-1-ov-file#readme\"><span style=\" text-decoration: underline; color:#007af4;\">BSL-1.0 license</span></a>): <a href=\"https://github.com/asmaloney/libE57Format\"><span style=\" text-decoration: underline; color:#007af4;\">https://github.com/asmaloney/libE57Format</span></a></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">LASZip (<a href=\"https://github.com/LASzip/LASzip?tab=Apache-2.0-1-ov-file#readme\"><span style=\" text-decoration: underline; color:#007af4;\">Apache 2</span></a>): <a href=\"https://github.com/LASzip/LASzip\"><span style=\" text-decoration: underline; color:#007af4;\">https://github.com/LASzip/LASzip</span></a></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Qt (<a href=\"https://www.qt.io/licensing/open-source-lgpl-obligations\"><span style=\" text-decoration: underline; color:#007af4;\">GPL / LGPL</span></a>): <a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#007af4;\">https://www.qt.io/</span></a></p>"
            "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>"
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Version: %1"
            "</p></body></html>").arg(_PROJECT_VERSION_);
    ui->textBrowser->setText(tbText);
}


Info::~Info()
{
    delete ui;
}
