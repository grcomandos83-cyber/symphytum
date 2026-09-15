/*
 *  Copyright (c) 2026 Symphytum Next Team
 */

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "currencyfieldwizard.h"
#include "../../components/metadataengine.h"
#include "../../utils/metadatapropertiesparser.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFrame>
#include <QtCore/QLocale>

//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------

CurrencyFieldWizard::CurrencyFieldWizard(const QString &fieldName,
                                         QWidget *parent,
                                         AbstractFieldWizard::EditMode editMode) :
    AbstractFieldWizard(fieldName, parent, editMode)
{
    setupUi();

    connect(m_backButton, &QPushButton::clicked, this, &AbstractFieldWizard::backSignal);
    connect(m_finishButton, &QPushButton::clicked, this, &AbstractFieldWizard::finishSignal);
    connect(m_currencyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CurrencyFieldWizard::onCurrencySelectionChanged);
    connect(m_customSymbolLineEdit, &QLineEdit::textChanged,
            this, &CurrencyFieldWizard::updatePreview);
    connect(m_positionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CurrencyFieldWizard::updatePreview);
    connect(m_precisionSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CurrencyFieldWizard::updatePreview);
    connect(m_markNegativeCheckBox, &QCheckBox::toggled,
            this, &CurrencyFieldWizard::updatePreview);

    updatePreview();
    m_finishButton->setFocus();
}

CurrencyFieldWizard::~CurrencyFieldWizard()
{
}

void CurrencyFieldWizard::getFieldProperties(QString &displayProperties,
                                             QString &editProperties,
                                             QString &triggerProperties)
{
    Q_UNUSED(triggerProperties);

    if (m_requiredFieldCheckBox->isChecked())
        displayProperties.append("markEmpty:1;");

    if (m_markNegativeCheckBox->isChecked())
        displayProperties.append("markNegative:1;");

    displayProperties.append(QString("precision:%1;").arg(m_precisionSpinBox->value()));

    QString symbol = getEffectiveSymbol();
    displayProperties.append(QString("currencySymbol:%1;").arg(symbol));

    QString pos = (m_positionComboBox->currentIndex() == 0) ? "suffix" : "prefix";
    displayProperties.append(QString("currencyPosition:%1;").arg(pos));

    if (m_requiredFieldCheckBox->isChecked())
        editProperties.append("noEmpty:1;");
}

void CurrencyFieldWizard::loadField(const int fieldId, const int collectionId)
{
    AbstractFieldWizard::loadField(fieldId, collectionId);

    MetadataEngine *meta = &MetadataEngine::getInstance();
    QString displayProps = meta->getFieldProperties(MetadataEngine::DisplayProperty,
                                                    fieldId, collectionId);
    MetadataPropertiesParser displayParser(displayProps);

    if (displayParser.size()) {
        if (displayParser.getValue("markNegative") == "1")
            m_markNegativeCheckBox->setChecked(true);

        QString prec = displayParser.getValue("precision");
        if (!prec.isEmpty())
            m_precisionSpinBox->setValue(prec.toInt());

        QString symbol = displayParser.getValue("currencySymbol");
        if (!symbol.isEmpty()) {
            int foundIdx = -1;
            for (int i = 0; i < m_currencyComboBox->count() - 1; ++i) {
                if (m_currencyComboBox->itemData(i).toString() == symbol) {
                    foundIdx = i;
                    break;
                }
            }
            if (foundIdx >= 0) {
                m_currencyComboBox->setCurrentIndex(foundIdx);
            } else {
                m_currencyComboBox->setCurrentIndex(m_currencyComboBox->count() - 1); // Custom
                m_customSymbolLineEdit->setText(symbol);
            }
        }

        QString pos = displayParser.getValue("currencyPosition");
        if (pos == "prefix")
            m_positionComboBox->setCurrentIndex(1);
        else
            m_positionComboBox->setCurrentIndex(0);
    }

    QString editProps = meta->getFieldProperties(MetadataEngine::EditProperty,
                                                 fieldId, collectionId);
    MetadataPropertiesParser editParser(editProps);
    if (editParser.size()) {
        if (editParser.getValue("noEmpty") == "1")
            m_requiredFieldCheckBox->setChecked(true);
    }

    updatePreview();
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void CurrencyFieldWizard::onCurrencySelectionChanged(int index)
{
    bool isCustom = (index == m_currencyComboBox->count() - 1);
    m_customSymbolLineEdit->setVisible(isCustom);
    m_customSymbolLineEdit->setEnabled(isCustom);
    if (isCustom)
        m_customSymbolLineEdit->setFocus();

    updatePreview();
}

void CurrencyFieldWizard::updatePreview()
{
    QString symbol = getEffectiveSymbol();
    int prec = m_precisionSpinBox->value();
    bool isSuffix = (m_positionComboBox->currentIndex() == 0);

    QLocale locale;
    QString samplePositive = QString::number(1234.56, 'f', prec);
    samplePositive.replace(".", locale.decimalPoint());

    QString sampleNegative = QString::number(-45.00, 'f', prec);
    sampleNegative.replace(".", locale.decimalPoint());

    QString formattedPos;
    QString formattedNeg;

    if (isSuffix) {
        formattedPos = QString("%1 %2").arg(samplePositive, symbol).trimmed();
        formattedNeg = QString("%1 %2").arg(sampleNegative, symbol).trimmed();
    } else {
        formattedPos = QString("%1 %2").arg(symbol, samplePositive).trimmed();
        formattedNeg = QString("%1 %2").arg(symbol, sampleNegative).trimmed();
    }

    QString previewText = QString("<b>%1:</b> %2").arg(tr("Positive"), formattedPos);
    if (m_markNegativeCheckBox->isChecked()) {
        previewText += QString("&nbsp;&nbsp;|&nbsp;&nbsp;<b>%1:</b> <span style='color:red;'>%2</span>")
                .arg(tr("Negative"), formattedNeg);
    } else {
        previewText += QString("&nbsp;&nbsp;|&nbsp;&nbsp;<b>%1:</b> %2")
                .arg(tr("Negative"), formattedNeg);
    }

    m_previewLabel->setText(previewText);
}

//-----------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------

void CurrencyFieldWizard::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *headerLabel = new QLabel(tr("3. Configure currency properties"), this);
    headerLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(headerLabel);

    // Checkboxes group
    QHBoxLayout *checksLayout = new QHBoxLayout();
    QGroupBox *generalGroup = new QGroupBox(tr("General"), this);
    QVBoxLayout *generalLayout = new QVBoxLayout(generalGroup);
    m_requiredFieldCheckBox = new QCheckBox(tr("Required field"), generalGroup);
    m_requiredFieldCheckBox->setToolTip(tr("Required fields must be filled out"));
    generalLayout->addWidget(m_requiredFieldCheckBox);
    checksLayout->addWidget(generalGroup);

    QGroupBox *valuesGroup = new QGroupBox(tr("Values"), this);
    QVBoxLayout *valuesLayout = new QVBoxLayout(valuesGroup);
    m_markNegativeCheckBox = new QCheckBox(tr("Mark negative values"), valuesGroup);
    m_markNegativeCheckBox->setToolTip(tr("Display negative currency values in red"));
    m_markNegativeCheckBox->setChecked(true);
    valuesLayout->addWidget(m_markNegativeCheckBox);
    checksLayout->addWidget(valuesGroup);

    mainLayout->addLayout(checksLayout);

    // Currency configuration group
    QGroupBox *currencyGroup = new QGroupBox(tr("Currency Formatting"), this);
    QFormLayout *formLayout = new QFormLayout(currencyGroup);

    m_currencyComboBox = new QComboBox(currencyGroup);
    m_currencyComboBox->addItem(QString::fromUtf8("€ (EUR)"), QString::fromUtf8("€"));
    m_currencyComboBox->addItem(QString::fromUtf8("$ (USD)"), QString::fromUtf8("$"));
    m_currencyComboBox->addItem(QString::fromUtf8("£ (GBP)"), QString::fromUtf8("£"));
    m_currencyComboBox->addItem(QString::fromUtf8("¥ (JPY)"), QString::fromUtf8("¥"));
    m_currencyComboBox->addItem(QString::fromUtf8("CHF (CHF)"), QString::fromUtf8("CHF"));
    m_currencyComboBox->addItem(tr("Custom..."), QString(""));

    m_customSymbolLineEdit = new QLineEdit(currencyGroup);
    m_customSymbolLineEdit->setPlaceholderText(tr("Enter custom symbol (e.g. kr, zł, ฿)"));
    m_customSymbolLineEdit->setVisible(false);

    QHBoxLayout *symbolRowLayout = new QHBoxLayout();
    symbolRowLayout->addWidget(m_currencyComboBox);
    symbolRowLayout->addWidget(m_customSymbolLineEdit);
    formLayout->addRow(tr("Currency symbol:"), symbolRowLayout);

    m_positionComboBox = new QComboBox(currencyGroup);
    m_positionComboBox->addItem(tr("Suffix (e.g. 10.00 €)"), "suffix");
    m_positionComboBox->addItem(tr("Prefix (e.g. $ 10.00)"), "prefix");
    formLayout->addRow(tr("Symbol position:"), m_positionComboBox);

    m_precisionSpinBox = new QSpinBox(currencyGroup);
    m_precisionSpinBox->setRange(0, 4);
    m_precisionSpinBox->setValue(2);
    formLayout->addRow(tr("Decimal places:"), m_precisionSpinBox);

    mainLayout->addWidget(currencyGroup);

    // Live preview
    QGroupBox *previewGroup = new QGroupBox(tr("Preview"), this);
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    m_previewLabel = new QLabel(previewGroup);
    m_previewLabel->setTextFormat(Qt::RichText);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    previewLayout->addWidget(m_previewLabel);
    mainLayout->addWidget(previewGroup);

    mainLayout->addStretch();

    // Separator line
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // Navigation buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();

    m_backButton = new QPushButton(tr("&Back"), this);
    buttonsLayout->addWidget(m_backButton);

    m_finishButton = new QPushButton(tr("&Finish"), this);
    m_finishButton->setDefault(true);
    buttonsLayout->addWidget(m_finishButton);

    mainLayout->addLayout(buttonsLayout);
}

QString CurrencyFieldWizard::getEffectiveSymbol() const
{
    if (m_currencyComboBox->currentIndex() == m_currencyComboBox->count() - 1) {
        QString custom = m_customSymbolLineEdit->text().trimmed();
        return custom.isEmpty() ? QString::fromUtf8("€") : custom;
    }
    return m_currencyComboBox->currentData().toString();
}
