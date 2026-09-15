/**
  * \class CurrencyFormWidget
  * \brief A form widget representing currency / monetary fields
  * \author Symphytum Next Team
  * \date 2026
  */

#ifndef CURRENCYFORMWIDGET_H
#define CURRENCYFORMWIDGET_H

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "abstractformwidget.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

class QLabel;
class QVBoxLayout;
class QLineEdit;

//-----------------------------------------------------------------------------
// CurrencyFormWidget
//-----------------------------------------------------------------------------

class CurrencyFormWidget : public AbstractFormWidget
{
    Q_OBJECT

public:
    CurrencyFormWidget(QWidget *parent = nullptr);

    void setFieldName(const QString &name);
    QString getFieldName() const;
    void clearData();
    void setData(const QVariant &data);
    QVariant getData() const;
    bool showHighlightSearchResults(const QString &searchString);

    /**
     * Supported display properties:
     * - currencySymbol: string (e.g. €, $, £)
     * - currencyPosition: suffix, prefix
     * - precision: int (0 to 4, default 2)
     * - markNegative: 1, 0
     * - markEmpty: 1, 0
     */
    void loadMetadataDisplayProperties(const QString &metadata);

protected slots:
    void validateData();

private:
    void updateStyleSheet();
    void setupFocusPolicy();
    QString formatValue(double value, bool empty) const;

    QLabel *m_fieldNameLabel;
    QLineEdit *m_lineEdit;
    QVBoxLayout *m_mainLayout;
    int m_precision;
    bool m_markNegative;
    bool m_markEmpty;
    QString m_currencySymbol;
    QString m_currencyPosition;
    double m_currentValue;
    bool m_hasValue;
};

#endif // CURRENCYFORMWIDGET_H
