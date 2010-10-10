set(stock_quote_engine_SRCS
    stock_quote_engine.cpp)

kde4_add_plugin(plasma_engine_stock_quote ${stock_quote_engine_SRCS})
target_link_libraries(plasma_engine_stock_quote
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${QT_QTNETWORK_LIBRARY})

install(TARGETS plasma_engine_stock_quote
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-dataengine-stockquote.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

