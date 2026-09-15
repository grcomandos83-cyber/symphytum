/**
  * \class CurrencyFieldWizard
  * \brief Wizard class for creation and configuration of currency type fields
  * \author Symphytum Next Team
  * \date 2026
  */

#ifndef CURRENCYFIELDWIZARD_H
#define CURRENCYFIELDWIZARD_H

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "abstractfieldwizard.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

class QCheckBox;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QLabel;
class QPushButton;

//-----------------------------------------------------------------------------
// CurrencyFieldWizard
//-----------------------------------------------------------------------------

class CurrencyFieldWizard : public AbstractFieldWizard
{
    Q_OBJECT

public:
    explicit CurrencyFieldWizard(const QString &fieldName,
                                 QWidget *parent = nullptr,
                                 AbstractFieldWizard::EditMode editMode
                                 = AbstractFieldWizard::NewEditMode);
    ~CurrencyFieldWizard();

    void getFieldProperties(QString &displayProperties,
                            QString &editProperties,
                            QString &triggerProperties) override;
    void loadField(const int fieldId, const int collectionId) override;

private slots:
    void onCurrencySelectionChanged(int index);
    void updatePreview();

private:
    void setupUi();
    QString getEffectiveSymbol() const;

    QCheckBox *m_requiredFieldCheckBox;
    QCheckBox *m_markNegativeCheckBox;
    QComboBox *m_currencyComboBox;
    QLineEdit *m_customSymbolLineEdit;
    QComboBox *m_positionComboBox;
    QSpinBox *m_precisionSpinBox;
    QLabel *m_previewLabel;
    QPushButton *m_backButton;
    QPushButton *m_finishButton;
};

#endif // CURRENCYFIELDWIZARD_H
