package com.guidewithme.async;

import java.io.IOException;
import java.io.InputStream;
import java.net.URLDecoder;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ZippedGuidesStorage
{
  private final ZipFile mZFile;

  public ZippedGuidesStorage(String filename)
  {
    try
    {
      mZFile = new ZipFile(filename);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

  public InputStream getData(String url)
  {
    try
    {
      final String decodedUrl = URLDecoder.decode(url, "utf-8");
      final ZipEntry ze = mZFile.getEntry(decodedUrl);
      return ze == null ? null : mZFile.getInputStream(ze);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

}
