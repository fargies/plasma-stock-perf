#include "stock_quote_engine.h"

#include <QDateTime>

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
StockQuoteEngine::StockQuoteEngine(QObject* parent, const QVariantList& args)
    :
    Plasma::DataEngine(parent, args),
    m_timer(),
    m_add_timer()
{
    Q_UNUSED(args);

//    setMinimumPollingInterval(30 * 1000);
//    setPollingInterval(5 * 60 * 1000);

    connect(&m_timer,     SIGNAL(timeout()), this, SLOT(update()));
    connect(&m_add_timer, SIGNAL(timeout()), this, SLOT(update()));

    // set a default update interval
    m_timer.setInterval(60 * 1000);

    // delay abit before trigger to allow the stocks to be added
    m_add_timer.setInterval(0.2);
    m_add_timer.setSingleShot(true);

    // create the status source
    setData("status", DataEngine::Data());
}

//------------------------------------------------------------------------------
// protected:
//------------------------------------------------------------------------------
bool
StockQuoteEngine::sourceRequestEvent(const QString& name)
{
    if ("status" != name)
    {
        setData(name, DataEngine::Data());

        // restart the main timer, and trigger the add timer
        m_timer.start();
        m_add_timer.start();
    }

    return true;
}

//------------------------------------------------------------------------------
// private slots:
//------------------------------------------------------------------------------
void
StockQuoteEngine::update()
{
    QStringList src = sources();
    src.pop_front(); // Remove "status" that's always in front

    const QString stocks = src.join(",");

    if (!stocks.isEmpty())
    {
        KUrl url("http://download.finance.yahoo.com");
        url.setPath("/d/quotes.csv");
        url.addQueryItem("f", "snl1c1");
        url.addQueryItem("e", ".csv");
        url.addQueryItem("s", stocks);

        KIO::TransferJob* job = KIO::get(url, KIO::Reload, KIO::HideProgressInfo);

        connect(job,  SIGNAL(data(KIO::Job*, const QByteArray&)),
                this, SLOT(dataArrived(KIO::Job*, const QByteArray&)));
        connect(job,  SIGNAL(result(KJob*)),
                this, SLOT(jobFinished(KJob*)));
    }
}

//------------------------------------------------------------------------------
void
StockQuoteEngine::dataArrived(KIO::Job* job, const QByteArray& data)
{
    Q_UNUSED(job);
    m_data.append(data);
}

void
StockQuoteEngine::jobFinished(KJob *job)
{
    Q_UNUSED(job);

    setData("status", "time", QDateTime::currentDateTime());
    setData("status", "valid", true);

    QStringList lines = m_data.split("\r\n");
    m_data.clear();

    for (QStringList::iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
        // if there is a commar in a quoted string, then we drop the commar as
        // it breaks the tokeniser
        *iter = fixQuotes(*iter);

        QStringList fields = iter->split(",");

        if (fields.size() != 4) {
          continue;
        }

        fields[0].remove(0, 1).chop(1);
        fields[1].remove(0, 1).chop(1);

        const QString symbol = fields[0];
        const QString name   = fields[1];
        const double  price  = fields[2].toDouble();
        const double  change = fields[3].toDouble();

        setData(symbol, "valid",          true);
        setData(symbol, "symbol",         symbol);
        setData(symbol, "name",           name);
        setData(symbol, "price",          price);
        setData(symbol, "change",         change);
    }
}

//------------------------------------------------------------------------------
// private:
//------------------------------------------------------------------------------
QString
StockQuoteEngine::fixQuotes(const QString& string)
{
    QString ret;
    bool quote = false;

    for (QString::const_iterator iter = string.begin(); iter != string.end(); ++iter)
    {
        if ('"' == *iter) quote = !quote;
        if (',' == *iter && true == quote) continue;

        ret += *iter;
    }

    return ret;
}

K_EXPORT_PLASMA_DATAENGINE(stockquote, StockQuoteEngine);

#include "stock_quote_engine.moc"

