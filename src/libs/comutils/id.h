#ifndef ID_H
#define ID_H

#include "comutils_global.h"

#include <QMetaType>
#include <QString>

class QDataStream;
class QVariant;

namespace CommonUtils {

class UTILS_EXPORT Id
{
public:
    enum { IdsPerPlugin = 10000, ReservedPlugins = 1000 };

    Id() : m_id(0) {}
    Id(int uid) : m_id(uid) {}
    Id(const char *name);

    Id withSuffix(int suffix) const;
    Id withSuffix(const char *suffix) const;
    Id withSuffix(const QString &suffix) const;
    Id withPrefix(const char *prefix) const;

    QByteArray name() const;
    QString toString() const; // Avoid.
    QVariant toSetting() const; // Good to use.
    QString suffixAfter(Id baseId) const;
    bool isValid() const { return m_id; }
    bool operator==(Id id) const { return m_id == id.m_id; }
    bool operator==(const char *name) const;
    bool operator!=(Id id) const { return m_id != id.m_id; }
    bool operator!=(const char *name) const { return !operator==(name); }
    bool operator<(Id id) const { return m_id < id.m_id; }
    bool operator>(Id id) const { return m_id > id.m_id; }
    bool alphabeticallyBefore(Id other) const;
    int uniqueIdentifier() const { return m_id; }
    static Id fromUniqueIdentifier(int uid) { return Id(uid); }
    static Id fromString(const QString &str); // FIXME: avoid.
    static Id fromName(const QByteArray &ba); // FIXME: avoid.
    static Id fromSetting(const QVariant &variant); // Good to use.
    static void registerId(int uid, const char *name);

private:
    // Intentionally unimplemented
    Id(const QLatin1String &);
    int m_id;
};


inline uint qHash(const Id &id) { return id.uniqueIdentifier(); }

} // namespace

Q_DECLARE_METATYPE(CommonUtils::Id)
//Q_DECLARE_METATYPE(QList<CommonUtils::Id>)

QDataStream &operator<<(QDataStream &ds, const CommonUtils::Id &id);
QDataStream &operator>>(QDataStream &ds, CommonUtils::Id &id);

#endif // ID_H
