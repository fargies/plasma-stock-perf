set(stock_quote_SRCS
    plasma_stock_quote.cpp
    stock_item.cpp
    overall_stock_item.cpp
    header_item.cpp
    footer_item.cpp
    stock_data.cpp
    stock_config.cpp)

kde4_add_ui_files(stock_quote_SRCS plasma_stock_quote_config.ui)
kde4_add_plugin(plasma_applet_stock_quote ${stock_quote_SRCS})

target_link_libraries(plasma_applet_stock_quote
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS})

install(TARGETS plasma_applet_stock_quote
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-stock-quote.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
