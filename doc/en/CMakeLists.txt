INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

ADD_SUBDIRECTORY(images)
ADD_SUBDIRECTORY(tutorials)

#Install our html files
FILE( GLOB SCRIBUS_DOC_EN_FILES *.html *.xml *.css )
INSTALL(FILES ${SCRIBUS_DOC_EN_FILES} DESTINATION ${DOCDIR}en)
