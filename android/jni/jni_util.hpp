#include <jni.h>

#include "../../std/string.hpp"

inline string JString2StdString(JNIEnv * env, jstring javaStr)
{
  string res;
  char const * buffer = env->GetStringUTFChars(javaStr, 0);
  if (buffer)
  {
    res = buffer;
    env->ReleaseStringUTFChars(javaStr, buffer);
  }
  return res;
}

inline jstring StdString2JString(JNIEnv * env, string const & stdString)
{
  return env->NewStringUTF(stdString.c_str());
}
