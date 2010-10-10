#pragma once

#include <Plasma/DataEngine>

#include <QTimer>

#include <kio/job.h>

class StockQuoteEngine : public Plasma::DataEngine
{
    Q_OBJECT

  public:
    StockQuoteEngine(QObject* parent, const QVariantList& args);

  protected:
    bool sourceRequestEvent(const QString& name);

  private slots:
    void update();
    void dataArrived(KIO::Job* job, const QByteArray& data);

  private:
    QString fixQuotes(const QString& string);

    QTimer m_timer;
    QTimer m_add_timer;
};

