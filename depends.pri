# This function sets up the dependencies for libraries that are built with
# this project.  Specify the libraries you need to depend on in the variable
# DEPENDENCY_LIBRARIES and this will add
for(sharedep, DEPENDENCY_SHARE_LIBRARIES) {
    #message($$TARGET depends on $$dep ($${DESTDIR}/$${dep}.$${SONAME}))
    LIBS += -L$${DESTDIR} -l$${sharedep}
    PRE_TARGETDEPS += $${DESTDIR}/lib$${sharedep}.so

    INCLUDEPATH += ../$${sharedep}
}


for(staticdep, DEPENDENCY_STATIC_LIBRARIES) {
    #message($$TARGET depends on $$dep ($${DESTDIR}/$${dep}.$${SONAME}))
    LIBS += -L$${STATIC_LIBS_PATH} -l$${staticdep}
    PRE_TARGETDEPS += $${STATIC_LIBS_PATH}/lib$${staticdep}.a
    INCLUDEPATH += ../$${staticdep}
}
