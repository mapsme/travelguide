#include "and_storage.hpp"
#include "jni_util.hpp"

/// @todo Replace on storage.hpp
#include "../../storage/storage_builder.hpp"


class AndStorage
{
public:

  static AndStorage & Instance()
  {
    static AndStorage storage;
    return storage;
  }

  void Query(string const & query, bool useLocation, double lat, double lon)
  {
    if (useLocation)
      m_storage.QueryArticleInfos(m_result, query, lat, lon);
    else
      m_storage.QueryArticleInfos(m_result, query);
  }

  int GetResultSize() const
  {
    return static_cast<int>(m_result.size());
  }

  ArticleInfo const & GetArticleInfoByIndex(int index) const
  {
    return m_result[index];
  }

  string GetParentName(ArticleInfo const & info)
  {
    return m_storage.GetParentName(info);
  }

private:
  /// @todo Replace on Storage
  StorageMock m_storage;
  vector<ArticleInfo> m_result;
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
JNIEXPORT void JNICALL Java_com_example_travelguide_cpp_Storage_query(JNIEnv * env, jobject thiz, jstring query,
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
  (JNIEnv * env, jobject thiz)
{
  return STORAGE.GetResultSize();
}

/*
 * Class:     com_example_travelguide_cpp_Storage
 * Method:    getArticleInfoByIndex
 * Signature: (I)Lcom/example/travelguide/article/ArticleInfo;
 */
JNIEXPORT jobject JNICALL Java_com_example_travelguide_cpp_Storage_getArticleInfoByIndex
  (JNIEnv * env, jobject thiz, jint index)
{
  ArticleInfo const & info = STORAGE.GetArticleInfoByIndex(index);
  jclass ArtInfoClass = env->FindClass("com/example/travelguide/article/ArticleInfo");
  jmethodID initId = env->GetMethodID(ArtInfoClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DD)V");

  return env->NewObject(ArtInfoClass, initId,
       StdString2JString(env, info.m_url),
       StdString2JString(env, info.m_thumbnailUrl),
       StdString2JString(env, info.m_title),
       StdString2JString(env, STORAGE.GetParentName(info)),
       info.m_lat,
       info.m_lon);
}


#ifdef __cplusplus
}
#endif
