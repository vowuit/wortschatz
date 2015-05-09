#ifndef TRAINERMODEL_H
#define TRAINERMODEL_H

#include "trainer.h"

#include <QObject>

class TrainerModel : public QObject
{
    Q_OBJECT

public:
    explicit TrainerModel(QObject *parent = 0);
    ~TrainerModel();

    virtual QString front() const = 0;
    virtual QString back() const = 0;

public slots:
    virtual bool answer(Trainer::Answer answer) = 0;
};

#endif // TRAINERMODEL_H
