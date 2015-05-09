#ifndef EMPTYTRAINERMODEL_H
#define EMPTYTRAINERMODEL_H

#include "trainermodel.h"

class EmptyTrainerModel : public TrainerModel
{
    Q_OBJECT

public:
    explicit EmptyTrainerModel(QObject *parent = 0);
    ~EmptyTrainerModel();

// TrainerModel interface
public:
    QString front() const;
    QString back() const;

public slots:
    bool answer(Trainer::Answer answer);
};

#endif // EMPTYTRAINERMODEL_H
