TEMPLATE      = subdirs
CONFIG       += ordered
SUBDIRS       = plugins \
                src

src.depends = plugins

