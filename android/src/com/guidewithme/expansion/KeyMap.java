package com.guidewithme.expansion;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class KeyMap
{
  final static Map<String, String> COUTRY_2_KEY = new HashMap<String, String>();

  private final static int KEY_LENGHT = 392;
  private final static int KEY_COUNT = 12;

  static
  {
    COUTRY_2_KEY.put("hawaii", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA14T4ru6Wj03NA8e/Gf4/MacpWjk+/j52lAe8Q5TWWlEeiRkvWYAWgT20O2rryGRTCNmgb69rDMch0QrXndenKucRHtG4q9xhF0KCeTFOpU3gHkP/oPmGTvhghDjCYRiQRNgcGDTfsrTDmyDwIaYJzNV9PbDht6e3UbhM+cN06QDTUreU9KSzXtNInXH9tZ4d4S3piO/f1tQRWTqUdNGav2xNaRsNrb61ZUyMdd4X+ap3c25xmsB4/snV61i/P5vEEeknzLaqI0KpCbxvGqSADTYfEycDue/+BXmVpcR7tsn3a/ebIBjUKzU09MwSrHDA01+BB03FF9w2PRUDLAFUdQIDAQAB");
    COUTRY_2_KEY.put("california", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEApcL+Gljmb5RtgCNrzzVJX6NehpniQhHL5YpbNGkY9ydPky/hTa3ThE6Y7fQiNnb1Bai695GPRAIYvHogbLV9YIJOYu1k3DpbrcwTJtSlhi827FS69pSUaGsDEVUxlxerjgCcRcqlYhZUbPi+5ACsQWVMqnHa+WTj3CfGAOuEH0CGhJk03QmMikC5jcWP9v4/ZthsSzeEwc9zX0DwxXzJ/tB0Tn7ot5EYzWGD49Lt0qMzDapmrQzLD2Avib8xtBTq76vXKB+GXKhnrDRQKsXThk00VHLwsj+j9CYgPbx3ExJKJjPSXslkbgCQxkxgFRe2CQG1qmOC0eWUZWugEIulFwIDAQAB");
    COUTRY_2_KEY.put("france", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtgHuz8EnzqJWMl3YE65hAC71FYOF3vM9cS5JieU42c3Yw5S3owvNgKda+fYo6h65tH+nM5yeHVkgYiRqdV6h0zAGWLH5NcFnIKlQZbp2gUWz4QIrmJPUnevGObVV84q3G0EmMXz1Fyn4KdydMaiE+9RzLn6BAKXPeXtu0XGiyqAGfc5vEVFJ2oDR0+2FFrchFF22tKAa2xe7m6sfUwSDopY8XwYtjUZttgGexTsHlETWelwWJRJs+9kmJCi5YDcUIhtq4UX1KJQTsq9ZToH8RmLLbFIaphqPazz0uNNrvCuVBp2OGzQqIAHVb3oCXCs+ESGROhcPnpZ5/4WMPf6jVwIDAQAB");
    COUTRY_2_KEY.put("germany", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhUM4Tl0QRKgbsiJVbnK1xPVoxZYl4s9LdfulKdhdkf1iI0iyW8e4cR9V6EzTrfpVIISckFu7acR6ObCPWxxLPZmBBE8CslyZtCx4+kI6lS3+hqEFbN+w4vYkGQtZl8Ie/oQmsSmYhmRdMtp1u7EEVLuMjHu0/bC8Rt8UIJV7NmzdipPpfM3ss+cKg2E1tU7w+W85zEUZy9UsgBNVx+KcGRMI5YPssdQ8AykCam4HT0EJ4w/eoR4D7qFLGE1HhX73cIPPzV6OFXRhxtnsxeokf1po0SMCkaJXOPVEolBZ3PMMYfWi/oSRDSthB02ndChIKzRuja+vZd6ip357ns3UkwIDAQAB");
    COUTRY_2_KEY.put("greece", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhppmFgHHGTa90mJ2eCT3iTCJj7pj7UbpN7Nt05xXykDj62HCqt4M2u4IC9e4HFr5R+qc/jF6I0sAb+/yV3nXY83MmzsSJGGKMkCItzAfv05bRCqNSUwk8eBZXMvHIXQXgA7mfK/effHYSbZSaaHajkkrbWZg87/oembsjkILgJfGLHc/vvHOFui903r3qqp40YCPXMXKjNAqZeDm4zewY8o3BWubhc6WvAau7TyXMTB0teq8DEvp16cdGYUhWjFiQMVp8X1UunRTZNz3bxwEQzOCtEph/m6VdQLE6fKDFETUIXpob23cy5f0W5iCEOu3Z+H5+JlBy51/6fC6ZUTK3wIDAQAB");
    COUTRY_2_KEY.put("italy", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmwCretR+l2z+nlah+St0EWtEyIbTzbxW2EUYspHtSMN7YtPLM48pW5jSyQ3WWWOx3N9Yr+woxxuHJ8OZxeJ05600JGiX9aLzsr3Tqh4o0KeisTFzbbLylLCkcBXfetaY0788yrkXb3KNdhH3drlqtzNXTJSf6VUBZlof8ml+3dixjyQwMNq8cQG7FpKFqDvvVJE+kHfU4itZe+GzXPp+3JSLgDpMo5X0+qPJ6hBTeA6EeNkuDUgIOkXoxlxooslEr2EaYsrSXP3tqMJ0lEFLqf/tMQDMWU2Ygw1BGPbD4pYZBVOmIGAzjxtuuSlVUggM/ekkcrTemKK8IzYQd4+KlwIDAQAB");
    COUTRY_2_KEY.put("japan", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAn1d9SrCfDmv4s4twnr5Y7m41bWEyClQhTJeIRF5DLo/WUPiZGbwAUCcFIKeScNeeFt4sAjoK5u3ORAymDRb3Y3JL6cm+wyc+sGrHJxfnNEFwNz9wMGWpgnnfliBdODLauFLXjJTBuXIXPqJqC3IO9ZdOzdCePQliOgDXMubi/1V5hY1x0KFdxfuGtY4JWLIBG5sPyDP0ODj2XOwhNyIaFbqfcoV8WwckGdDGvsx4qavLZ3fKLEF+txVKIkdgZn1Gy2hKS3yKp+DmEY1dXMTXi4iIiR/osEVElD4H9cyWc+ZN7bUNL8eBD85GQv8GyH9OLRS5LRG2vzBwUQ87kj7t8wIDAQAB");
    COUTRY_2_KEY.put("russia", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAknN1T57PpDn9ToHY+m3Y8m/r9WsVugl67Gyl0U7Fta7fT+j4s+GqfpTe7sHAi/zfTN+9LjkttqI8b53pj9g+DBx3TNyNHoSYaokiwLomEA8p7LJAS5vSHDzL0kMs2wOa8MHViwWacKISrekj9RD/8HSk9MuCp9f7BlOAS36j/MW5YV1qMbNSB1gejGQb9zVsc0PphN1wdeounO8vvGmfLhKGEZL5rSqnahgQJFjaXlWGv79xFkRWixyiQyD+eJUKMMpgg5TttypaHosIQGrVbspe+oWTfJiTakwlglAEZle4VbltzJnhF/ywRYq7IchW7BrjsrCwa23Z+p6hGOd32QIDAQAB");
    COUTRY_2_KEY.put("spain", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnwbrpx9R+ZjdcVqiA7wBgfU9LOCGncV2H2baapdew4euKMCoK0+bMIDHZceIyc7aQJF7KGjPprbw6pUUziwzLHYH5yp8oP4d5bXbL1nZaUMCf2YYeEUpdW7iNjqtfMlv6AqvpC1TjNg/jrkoWgPK9ojWQBLlH+IYVBhGhmwT1UfW031YByp+UVhSHoyC3vUPCEXo2S/KIzd9r+h7JyGImTdmbfRkf2eJDcrwvs5OtCjJ+4ch8Xbrmy5Sr3nZg6ZziAvBDFRIeHbybboqesdhknvEgHVo9iUSD8ltBD9X2RkQpBXvoIS+Uq4tAcR/3i3fSQ7WUF0tbe508S6np5QvHQIDAQAB");
    COUTRY_2_KEY.put("switzerland", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAn5s5bicbujNjm8IdqTT87TISYgIHEilt9cjl/pcfpKiIXA+e6VBl1ePdaZoQiS1g2nN4a1tLTtfe6QVwsZpwxt4NogpOsBTd1LNsUjRPdqI3Co2Yb9KhaHeLVrw6wuXJvDrmdvHo4lq3B791PVzL7kmAJflGL/MDbiJrKDI7Kea2Ah00igZw/xy01wLklYx++oXYRgXTZYtLjLWJu0cxIYIZ+FstXR91Sgk/zsudycEZdFCsHAbggJF8nBvzCfOfaTGnUem7Jc0zYLvUf//ZUwpJtkwrtAb+THv2Isnc2ZkE9Lv+HnJXmvx0k9ZI0P5fccmjscFOpzWIgqt56f7nbwIDAQAB");
    COUTRY_2_KEY.put("turkey", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0wOCJ6FwP+6EclHeZOLvYqDq3gml/16SxdtobkNP7qxYjGJfmjoxYWIa2XxgmgHmJEanKCtNbzpW++seP4f/9FJzepy4UMNrzubpK24yvOpHssMNjEiKcrT/rfMznbiPAMcLw7AMU2iulk+TWbmQD9Roj/sx0nANkO9FZ6kHkzBy4Z5qjGOqDFzhKdVWHQrjKIUWCEs1kRsT6EEkdDV+MKRl18s/tgA3FqSil7GCGko5aArF0EJWkbNryiwpfUMIQ3x2k66YRXzEFHwGm+26V+fMV3Bm3yBfkE7yC81KeiVrIXvSduOBn8j0bQd2qhtKc7JR8KZ/s6JmNASicWEs6QIDAQAB");
    COUTRY_2_KEY.put("uk", "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAu1XiE4QTmYZvKH5VPu5RwSeW0B5SzGyVGFc9fBona0J7gNHGBb7zW6MJzxrnen725/JvWCsQQPjEbfGQGn8SZcxCrKMcN4rBQLtw66fmllEJgtFORZMep0WlH+8Wk9Ss4CvYwnvwUsHtTRAQbFKYBbaGVHZwZBKDyVNzTAuM+jQo8RPocbkbhv8bcBy3z4oolAMhBZghAIOBafi608zv+AVW3L1SJZ9gWrVC/+F5YlxXV4+7npImHjThwB9ZDjWkSJZjlQSiDkNf6H0PnyMv+yymyOumXMoiXW4uLTGAWXhpUxrLuIpAd8M2lDU3Ofwxz2tMkd7QuB/ZSgEmlyCs0QIDAQAB");

    validateKeys();
  }

  private static void validateKeys()
  {
    if (COUTRY_2_KEY.size() != KEY_COUNT)
      throw new CopyPasteException(String
          .format("There must be %d key, found %d keys", KEY_COUNT, COUTRY_2_KEY.size()));

    // check values are different and have appropriate length
    final Set<String> rsaKeysSet = new HashSet<String>();
    for (final String country : COUTRY_2_KEY.keySet())
    {
      final String rsaKey = COUTRY_2_KEY.get(country);

      if (rsaKey.length() != KEY_LENGHT)
        throw new CopyPasteException(String
            .format("Key length %d is incorrect for %s", rsaKey.length(), country));

      if (rsaKeysSet.contains(rsaKey))
        throw new CopyPasteException(String
            .format("Possible key duplication for %s", country));

      rsaKeysSet.add(rsaKey);
    }
  }

  public static class CopyPasteException extends RuntimeException
  {
    private static final long serialVersionUID = 4067230949552311109L;

    public  CopyPasteException(String message)
    {
      super(message);
    }
  }
}
