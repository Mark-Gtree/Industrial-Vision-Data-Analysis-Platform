#ifndef EXCELMANGER_H
#define EXCELMANGER_H

#include <QWidget>
#include <QString>
#include <QAxObject>
#include <QDialog>

class ExcelManger : public QWidget
{
    Q_OBJECT
public:
    explicit ExcelManger(QWidget *parent = nullptr);
    ~ExcelManger();

protected:
    static QAxObject *excel;
    static QAxObject *workbooks;
    static QAxObject *workbook;
    static int count;

    void new_excel(const QString&);
    void open_excel(const QString&);
    QString get_cell_value(QAxObject*, int, int);
    QVariant get_range(QAxObject*, const QString&);
    void set_cell_value(QAxObject*, int, int, const QString&);
    void merge_cells(QAxObject*, const QString&);
    void set_cell_font_bold(QAxObject *sheet, const QString &cell, bool isBold = true);
    void set_cell_font_center(QAxObject *sheet, const QString &cell);
    void set_rows_autofit(QAxObject *sheet, const QString &rows);
    void set_cols_autofit(QAxObject *sheet, const QString &cols);
    void save_excel();
    void save_excel_as(const QString&);
    void close();
    void free_excel();

signals:

};

#endif // EXCELMANGER_H
