#include "excelmanger.h"
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QDir>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

QAxObject* ExcelManger::excel = nullptr;
QAxObject* ExcelManger::workbooks = nullptr;
QAxObject* ExcelManger::workbook = nullptr;
int ExcelManger::count = 0;

ExcelManger::ExcelManger(QWidget *parent) : QWidget(parent)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if ((count++) == 0)
    {
        excel = new QAxObject("Excel.Application", this->parent());
        excel->dynamicCall("SetVisible(bool)", false);
        workbooks = excel->querySubObject("WorkBooks");
    }
}

ExcelManger::~ExcelManger()
{
    if ((--count) == 0)
        free_excel();
}

void ExcelManger::new_excel(const QString &path)
{
    workbooks->dynamicCall("Add");
    workbook = excel->querySubObject("ActiveWorkBook");
    save_excel_as(path);
}

void ExcelManger::open_excel(const QString &path)
{
    close();
    QFile file(path);
    if (!file.exists())
        new_excel(path);
    else
        workbook = workbooks->querySubObject("Open(QString&)", path);
    file.close();
}

QString ExcelManger::get_cell_value(QAxObject *sheet, int row, int col)
{
    QAxObject *range = sheet->querySubObject("Cells(int,int)", row, col);
    return range->property("Value").toString();
}

QVariant ExcelManger::get_range(QAxObject *sheet, const QString &range)
{
    return sheet->querySubObject("Range(const QString&)", range)->property("value");
}

void ExcelManger::set_cell_value(QAxObject *sheet, int row, int col, const QString& value)
{
    QAxObject *range = sheet->querySubObject("Cells(int,int)", row, col);
    range->dynamicCall("Value", value);
}

void ExcelManger::merge_cells(QAxObject *sheet, const QString &cell)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

void ExcelManger::set_cell_font_bold(QAxObject *sheet, const QString &cell, bool isBold)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void ExcelManger::set_cell_font_center(QAxObject *sheet, const QString &cell)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);
    range->setProperty("VerticalAlignment", -4108);
}

void ExcelManger::set_rows_autofit(QAxObject *sheet, const QString &rows)
{
    QAxObject *Rows = sheet->querySubObject("Rows(const QString &)", rows);
    Rows->dynamicCall("AutoFit()");
}

void ExcelManger::set_cols_autofit(QAxObject *sheet, const QString &cols)
{
    QAxObject *Cols = sheet->querySubObject("Columns(const QString &)", cols);
    Cols->dynamicCall("AutoFit()");
}

void ExcelManger::save_excel_as(const QString &path)
{
    if (workbook)
        workbook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(path));
}

void ExcelManger::save_excel()
{
    if (workbook)
        workbook->dynamicCall("Save()");
}

void ExcelManger::close()
{
    if (workbook)
        workbook->dynamicCall("Close()");
}

void ExcelManger::free_excel()
{
    if (excel != nullptr)
    {
        workbooks->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        delete workbook;
        delete workbooks;
        delete excel;
        excel = nullptr;
        workbooks = nullptr;
        workbook = nullptr;
    }
}
