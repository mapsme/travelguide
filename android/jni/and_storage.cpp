#include "and_storage.hpp"
#include "jni_util.hpp"

#include "../../storage/storage.hpp"

#include "../../env/reader.hpp"
#include "../../env/latlon.hpp"

#include <android/asset_manager_jni.h>


class AndStorage
{
  class AssetReader : public rd::Reader
  {
    AAsset * m_handle;
  public:
    AssetReader(char const * name, AAssetManager * manager)
    {
      m_handle = AAssetManager_open(manager, name, AASSET_MODE_BUFFER);
      CHECK(m_handle, ());
    }

    ~AssetReader()
    {
      AAsset_close(m_handle);
    }

    virtual void Read(void * p, size_t size)
    {
      AAsset_read(m_handle, p, size);
    }
  };

public:
  void Init(JNIEnv * env, jobject manager)
  {
    AssetReader reader("index.dat", AAssetManager_fromJava(env, manager));
    m_storage.Load(reader);
  }

  static AndStorage & Instance()
  {
    static AndStorage storage;
    return storage;
  }

  void Query(string const & query, bool useLocation, double lat, double lon)
  {
    if (useLocation)
      m_storage.QueryArticleInfo(query, lat, lon);
    else
      m_storage.QueryArticleInfo(query);
  }

  int GetResultSize() const
  {
    return static_cast<int>(m_storage.GetResultsCount());
  }

  ArticleInfo const & GetArticleInfoByIndex(int index) const
  {
    return m_storage.GetResult(index);
  }

  string GetParentName(ArticleInfo const & info) const
  {
    return m_storage.FormatParentName(info);
  }

  ArticleInfo const * GetArticleInfoByUrl(string const & url)
  {
    return m_storage.GetArticleInfoFromUrl(url);
  }

private:
  Storage m_storage;
};

#ifdef __cplusplus
extern "C"
{
#endif

// TODO: *problem
//#define DECLARE_FN(retType, suffix)

#define STORAGE AndStorage::Instance()

/*
 * Class:     com_example_travelguide_cpp_Storage
 * Method:    query
 * Signature: (Ljava/lang/String;DD)V
 */
JNIEXPORT void JNICALL Java_com_example_travelguide_cpp_Storage_query(JNIEnv * env, jclass clazz, jstring query,
    jboolean useLocation, jdouble lat, jdouble lon)
{
  STORAGE.Query(JString2StdString(env, query), useLocation, lat, lon);
}

/*
 * Class:     com_example_travelguide_cpp_Storage
 * Method:    getResultSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_travelguide_cpp_Storage_getResultSize
  (JNIEnv * env, jclass clazz)
{
  return STORAGE.GetResultSize();
}

jobject NativeArticle2JavaArticle(JNIEnv * env, ArticleInfo const * p)
{
  if (p == 0)
    return 0;

  jclass ArtInfoClass = env->FindClass("com/example/travelguide/article/ArticleInfo");
  jmethodID initId = env->GetMethodID(ArtInfoClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DD)V");

  return env->NewObject(ArtInfoClass, initId,
       StdString2JString(env, p->GetUrl()),
       StdString2JString(env, p->GetThumbnailUrl()),
       StdString2JString(env, p->GetTitle()),
       StdString2JString(env, STORAGE.GetParentName(*p)),
       p->m_lat,
       p->m_lon);
}

/*
 * Class:     com_example_travelguide_cpp_Storage
 * Method:    getArticleInfoByIndex
 * Signature: (I)Lcom/example/travelguide/article/ArticleInfo;
 */
JNIEXPORT jobject JNICALL Java_com_example_travelguide_cpp_Storage_getArticleInfoByIndex
  (JNIEnv * env, jclass clazz, jint index)
{
  return NativeArticle2JavaArticle(env, &STORAGE.GetArticleInfoByIndex(index));
}

JNIEXPORT void JNICALL Java_com_example_travelguide_cpp_Storage_nativeInitIndex
  (JNIEnv * env, jclass clazz, jobject assetManager)
{
  STORAGE.Init(env, assetManager);
}

JNIEXPORT jobject JNICALL Java_com_example_travelguide_cpp_Storage_getArticleInfoByUrl
  (JNIEnv * env, jclass clazz, jstring url)
{
  return NativeArticle2JavaArticle(env, STORAGE.GetArticleInfoByUrl(JString2StdString(env, url)));
}

JNIEXPORT jboolean JNICALL Java_com_example_travelguide_cpp_Storage_isValidLatLon
  (JNIEnv * env, jclass clazz, jdouble lat, jdouble lon)
{
  return ll::ValidLat(lat) && ll::ValidLon(lon) ? JNI_TRUE : JNI_FALSE;
}


#ifdef __cplusplus
}
#endif
