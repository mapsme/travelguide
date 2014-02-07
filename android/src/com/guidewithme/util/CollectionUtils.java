package com.guidewithme.util;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.NoSuchElementException;

public class CollectionUtils
{
  public interface Predicate<T>
  {
    boolean apply(T arg);
  }

  public static <T> List<T> filter(Collection<T> input, Predicate<T> condition)
  {
    final ArrayList<T> out = new ArrayList<T>(input.size());
    for (final T val : input)
    {
      if (condition.apply(val))
        out.add(val);
    }
    return out;
  }

  public static <T> List<T> filter(Enumeration<T> input, Predicate<T> condition)
  {
    return filter(Collections.list(input), condition);
  }

  public static <T> T any(List<T> input)
  {
    if (input.isEmpty())
      throw new NoSuchElementException("Input collection is empty");

    return input.get(Utils.random(input.size()));
  }
}
