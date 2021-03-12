function isAlphanumeric(str, locale = 'fr-FR') {
  if (locale in alphanumeric) { 
    return isAlphanumeric[locale].test(str);
  }
  else { ExceptionAdmin("definition structure" + locale + "Invalide" );   return false ; }; 
};
