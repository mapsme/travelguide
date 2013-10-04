cache()

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = env \
          storage \
          builder/genindex \

# not fail build!
