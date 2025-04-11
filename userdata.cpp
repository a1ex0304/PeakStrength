#include "userdata.h"
#include <QDebug>

UserData::UserData()
{
    loadData();
}

bool UserData::createUser(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty() || userExists(username))
        return false;

    usernames.append(username);
    passwords.append(password);
    workoutHistories.append(QVector<QString>());
    
    return saveData();
}

bool UserData::deleteUser(const QString &username)
{
    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == username) {
            usernames.remove(i);
            passwords.remove(i);
            workoutHistories.remove(i);
            return saveData();
        }
    }
    return false;
}

bool UserData::userExists(const QString &username) const
{
    return usernames.contains(username);
}

bool UserData::validateUser(const QString &username, const QString &password)
{
    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == username && passwords[i] == password)
            return true;
    }
    return false;
}

void UserData::addWorkout(const QString &username, const QString &workout)
{
    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == username) {
            workoutHistories[i].append(workout);
            saveData();
            return;
        }
    }
}

bool UserData::deleteWorkout(const QString &username, int index)
{
    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == username) {
            if (index >= 0 && index < workoutHistories[i].size()) {
                workoutHistories[i].remove(index);
                return saveData();
            }
        }
    }
    return false;
}

QVector<QString> UserData::getWorkouts(const QString &username)
{
    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == username) {
            return workoutHistories[i];
        }
    }
    return QVector<QString>();
}

bool UserData::loadData()
{
    QFile file(dataFile);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    usernames.clear();
    passwords.clear();
    workoutHistories.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        
        if (parts.size() >= 2) {
            usernames.append(parts[0]);
            passwords.append(parts[1]);
            
            QVector<QString> workouts;
            if (parts.size() > 2) {
                QStringList workoutList = parts[2].split(';');
                for (const QString &workout : workoutList) {
                    if (!workout.isEmpty())
                        workouts.append(workout);
                }
            }
            workoutHistories.append(workouts);
        }
    }
    file.close();
    return true;
}

bool UserData::saveData()
{
    QFile file(dataFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QTextStream out(&file);
    for (int i = 0; i < usernames.size(); ++i) {
        out << usernames[i] << "," << passwords[i] << ",";
        out << workoutHistories[i].join(";") << "\n";
    }
    file.close();
    return true;
}