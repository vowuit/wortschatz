#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QUrl>

class TrainerModel;

class Trainer : public QObject
{
    Q_OBJECT

    Q_ENUMS(Answer)

    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

    Q_PROPERTY(QString front READ front NOTIFY frontChanged)
    Q_PROPERTY(QString back READ back NOTIFY backChanged)

public:
    explicit Trainer(QObject *parent = 0);
    ~Trainer();

    enum Answer { YES, NO, SUCCESS, FAIL, CORRECT, WRONG, EASY, TOUGH, A, B, C, D, E, F, G  };

    bool open(QUrl url);
    bool close();

    TrainerModel *trainerModel() const;

    QUrl url() const;

    QString front() const;
    QString back() const;

signals:
    void urlChanged();
    void trainerModelChanged();

    void frontChanged();
    void backChanged();

public slots:
    bool answer(Trainer::Answer answer);

private:
    TrainerModel *m_trainerModel;
    QUrl m_url;
};

#endif // TRAINER_H
