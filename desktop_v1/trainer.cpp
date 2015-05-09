#include <QFileInfo>

#include "trainer.h"
#include "trainermodel.h"
#include "emptytrainermodel.h"
#include "sqlitetrainermodel.h"

Trainer::Trainer(QObject *parent) :
    QObject(parent), m_trainerModel(new EmptyTrainerModel(this))
{
    m_url = QUrl();
}

Trainer::~Trainer()
{

}

bool Trainer::open(QUrl url)
{
    Q_UNUSED(url)

    QFileInfo fileInfo(url.toLocalFile());

    if(fileInfo.exists()) {
        SqliteTrainerModel *model = new SqliteTrainerModel(this,fileInfo.filePath());

        m_url = url;

        m_trainerModel->deleteLater();
        m_trainerModel = model;

        emit urlChanged();
        emit trainerModelChanged();

        emit frontChanged();
        emit backChanged();
    }

    return true;
}

bool Trainer::close()
{
    m_trainerModel->deleteLater();
    m_trainerModel = new EmptyTrainerModel(this);

    m_url = QUrl();
    emit urlChanged();
    emit trainerModelChanged();

    emit frontChanged();
    emit backChanged();

    return true;
}

TrainerModel *Trainer::trainerModel() const
{
    return m_trainerModel;
}

QUrl Trainer::url() const
{
    return m_url;
}

QString Trainer::front() const
{
    return m_trainerModel->front();
}

QString Trainer::back() const
{
    return m_trainerModel->back();
}

bool Trainer::answer(Trainer::Answer answer)
{
    bool ok = m_trainerModel->answer(answer);

    emit frontChanged();
    emit backChanged();

    return ok;
}

