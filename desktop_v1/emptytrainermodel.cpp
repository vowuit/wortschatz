#include "emptytrainermodel.h"

EmptyTrainerModel::EmptyTrainerModel(QObject *parent) :
    TrainerModel(parent)
{

}

EmptyTrainerModel::~EmptyTrainerModel()
{

}

QString EmptyTrainerModel::front() const
{
    return QString();
}

QString EmptyTrainerModel::back() const
{
    return QString();
}

bool EmptyTrainerModel::answer(Trainer::Answer answer)
{
    Q_UNUSED(answer)
    return false;
}

