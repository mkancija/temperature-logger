/**
 * Inicijalizacija i globalne varijable za 18b20 senzore
 *
 */

String buildEthernetData()
{

  // sensors.requestTemperatures();
  String tx;
  tx = String("&v1=")+String(t18_t1)+String("&v2=")+String(t18_t2)+String("&v3=")+String(bmp_t1)+String("&v4=")+String(dht_h1)+String("&v5=")+String(dht_t1)+String("&v6=")+String(t18_t3);
  tx += String("&v12=")+String(bmp_p1);

  return tx;
}

 
