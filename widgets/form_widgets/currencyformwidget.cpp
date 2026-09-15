/*
 *  Copyright (c) 2026 Symphytum Next Team
 */

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "currencyformwidget.h"
#include "../../utils/platformcolorservice.h"
#include "../../utils/metadatapropertiesparser.h"
#include "../../utils/formwidgetvalidator.h"
#include "../../components/metadataengine.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QVariant>
#include <QtCore/QLocale>
#include <QtCore/QRegularExpression>

//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------

CurrencyFormWidget::CurrencyFormWidget(QWidget *parent) :
    AbstractFormWidget(parent),
    m_precision(2),
    m_markNegative(false),
    m_markEmpty(false),
    m_currencySymbol(QString::fromUtf8("€")),
    m_currencyPosition("suffix"),
    m_currentValue(0.0),
    m_hasValue(false)
{
    m_fieldNameLabel = new QLabel("Invalid Name", this);
    m_mainLayout = new QVBoxLayout(this);
    m_lineEdit = new QLineEdit(this);

    m_fieldNameLabel->setStyleSheet("QLabel {color: gray;}");
    m_lineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);
    m_lineEdit->setAlignment(Qt::AlignRight);

    m_mainLayout->addWidget(m_fieldNameLabel);
    m_mainLayout->addWidget(m_lineEdit);
    m_mainLayout->addStretch();

    this->heightUnits = 1;
    this->widthUnits = 1;

    connect(m_lineEdit, &QLineEdit::editingFinished,
            this, &CurrencyFormWidget::validateData);

    setupFocusPolicy();
}

void CurrencyFormWidget::setFieldName(const QString &name)
{
    m_fieldNameLabel->setText(name);
}

QString CurrencyFormWidget::getFieldName() const
{
    return m_fieldNameLabel->text();
}

void CurrencyFormWidget::clearData()
{
    m_hasValue = false;
    m_currentValue = 0.0;
    m_lineEdit->clear();
    updateStyleSheet();
}

void CurrencyFormWidget::setData(const QVariant &data)
{
    bool empty = data.toString().trimmed().isEmpty();
    if (empty) {
        clearData();
    } else {
        bool ok = false;
        double d = data.toDouble(&ok);
        if (!ok) {
            // try locale-aware parse
            QLocale locale;
            d = locale.toDouble(data.toString(), &ok);
        }
        if (ok) {
            m_currentValue = d;
            m_hasValue = true;
            m_lineEdit->setText(formatValue(d, false));
        } else {
            m_currentValue = 0.0;
            m_hasValue = false;
            m_lineEdit->setText(data.toString());
        }
    }

    updateStyleSheet();
}

QVariant CurrencyFormWidget::getData() const
{
    QString s = m_lineEdit->text().trimmed();
    if (s.isEmpty())
        return "";

    // Remove currency symbol if present
    if (!m_currencySymbol.isEmpty())
        s.remove(m_currencySymbol);
    s = s.trimmed();

    // Determine which of '.' or ',' is decimal point
    QLocale locale;
    QChar notDecimal = (locale.decimalPoint() == '.') ? ',' : '.';

    // Remove anything that isn't a digit, negative sign, or decimal point
    s.remove(QRegularExpression(QString("[^0-9\\-%1]").arg(locale.decimalPoint())));
    s.replace(notDecimal, locale.decimalPoint());

    bool ok = false;
    double d = locale.toDouble(s, &ok);
    if (!ok) {
        d = s.toDouble(&ok);
    }

    if (ok)
        return d;
    else
        return "";
}

bool CurrencyFormWidget::showHighlightSearchResults(const QString &searchString)
{
    bool r = m_lineEdit->text().contains(searchString, Qt::CaseInsensitive);
    QString highLightCSS = "QLabel { background: yellow; } QLineEdit { color: red; }";
    QString currentStyleSheet = this->styleSheet();
    if (r) {
        if (!currentStyleSheet.contains(highLightCSS))
            this->setStyleSheet(currentStyleSheet.append(highLightCSS));
    } else {
        this->setStyleSheet(currentStyleSheet.remove(highLightCSS));
    }

    return r;
}

void CurrencyFormWidget::loadMetadataDisplayProperties(const QString &metadata)
{
    if (metadata.isEmpty()) return;

    MetadataPropertiesParser parser(metadata);

    QString sym = parser.getValue("currencySymbol");
    if (!sym.isEmpty())
        m_currencySymbol = sym;

    QString pos = parser.getValue("currencyPosition");
    if (!pos.isEmpty())
        m_currencyPosition = pos;

    QString prec = parser.getValue("precision");
    bool ok = false;
    int p = prec.toInt(&ok);
    if (ok)
        m_precision = p;

    if (parser.getValue("markNegative") == "1")
        m_markNegative = true;

    if (parser.getValue("markEmpty") == "1")
        m_markEmpty = true;
}

//-----------------------------------------------------------------------------
// Protected slots
//-----------------------------------------------------------------------------

void CurrencyFormWidget::validateData()
{
    QString editMetadata = MetadataEngine::getInstance().getFieldProperties(
                MetadataEngine::EditProperty, getFieldId());
    FormWidgetValidator validator(editMetadata, MetadataEngine::CurrencyType);
    QString errorMessage;

    QVariant val = this->getData();
    bool valid = validator.validate(val, errorMessage);

    if (valid) {
        if (!val.toString().isEmpty()) {
            m_currentValue = val.toDouble();
            m_hasValue = true;
            m_lineEdit->setText(formatValue(m_currentValue, false));
        } else {
            m_hasValue = false;
            m_lineEdit->clear();
        }
        updateStyleSheet();
        emit dataEdited();
    } else {
        m_lineEdit->undo();
        updateStyleSheet();
        emit requiresAttention(errorMessage);
    }
}

//-----------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------

QString CurrencyFormWidget::formatValue(double value, bool empty) const
{
    if (empty) return "";

    QLocale locale;
    QString numStr = QString::number(value, 'f', m_precision);
    numStr.replace(".", locale.decimalPoint());

    if (m_currencyPosition == "prefix") {
        return QString("%1 %2").arg(m_currencySymbol, numStr).trimmed();
    } else {
        return QString("%1 %2").arg(numStr, m_currencySymbol).trimmed();
    }
}

void CurrencyFormWidget::updateStyleSheet()
{
    QColor c = PlatformColorService::getHighlightColor();
    QString style;

    style.append("QLineEdit { border-radius: 7px; padding: 5px; ");

    if (m_markNegative && m_hasValue && (m_currentValue < 0.0)) {
        style.append("color: red; ");
    }

    if (m_markEmpty && !m_hasValue) {
        style.append("background-color: #FFDFDF; border: 2px solid #FF7979; ");
    } else {
        style.append("border: 2px solid lightgray; ");
    }

    style.append(QString("} QLineEdit:focus { border: 2px solid rgb(%1, %2, %3); }")
                 .arg(c.red()).arg(c.green()).arg(c.blue()));

    m_lineEdit->setStyleSheet(style);
}

void CurrencyFormWidget::setupFocusPolicy()
{
    m_lineEdit->setFocusPolicy(Qt::ClickFocus);
    setFocusProxy(m_lineEdit);
    setFocusPolicy(Qt::StrongFocus);
}
