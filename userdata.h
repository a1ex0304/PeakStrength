#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

class UserData
{
public:
    UserData();

    // User management
    bool createUser(const QString &username, const QString &password);
    bool deleteUser(const QString &username);
    bool userExists(const QString &username) const;
    bool validateUser(const QString &username, const QString &password);

    // Workout history
    void addWorkout(const QString &username, const QString &workout);
    bool deleteWorkout(const QString &username, int index);
    QVector<QString> getWorkouts(const QString &username);

private:
    const QString dataFile = "users.csv";
    QVector<QString> usernames;
    QVector<QString> passwords;
    QVector<QVector<QString>> workoutHistories;

    bool loadData();
    bool saveData();
};

#endif // USERDATA_H