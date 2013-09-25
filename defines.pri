# Common file for all projects, to avoid copy-paste

CONFIG(release, debug|release) {
  DEFINES *= RELEASE _RELEASE NDEBUG
} else {
  DEFINES *= DEBUG _DEBUG
}
