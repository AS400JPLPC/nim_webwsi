
//---------- Message for administrateur
function ExceptionAdmin(func,errorMsg){
	alert("definition structure  ->"+func+"--"+errorMsg+"<- Invalide");}

//---------- change value to string
function toString(input){
	if(typeof input==='object'&&input!==null){
		if(typeof input.toString==='function'){
			input=input.toString();}
			else{input='[object Object]';}
	}
	else if(input===null||typeof input==='undefined'||(isNaN(input)&&!input.length)){input='';};
return String(input);
};


var alpha={		//	Only a-Z
	'en-US':/^[A-Z]+$/i,
	'cs-CZ':/^[A-ZÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ]+$/i,
	'de-DE':/^[A-ZÄÖÜß]+$/i,
	'es-ES':/^[A-ZÁÉÍÑÓÚÜ]+$/i,
	'fr-FR':/^[A-ZÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ]+$/i,
	'nl-NL':/^[A-ZÉËÏÓÖÜ]+$/i,
	'hu-HU':/^[A-ZÁÉÍÓÖŐÚÜŰ]+$/i,
	'pl-PL':/^[A-ZĄĆĘŚŁŃÓŻŹ]+$/i,
	'pt-PT':/^[A-ZÃÁÀÂÇÉÊÍÕÓÔÚÜ]+$/i,
	'ru-RU':/^[А-ЯЁа-яё]+$/i,
	'tr-TR':/^[A-ZÇĞİıÖŞÜ]+$/i,
	'he':/^[\u0590-\u05FF]+$/,
	'iw':/^[\u0590-\u05FF]+$/,
	'ar':/^[\u0600-\u06FF]+$/,
	};

var alphaname={		//	Only a-Z 'esp -'
	'en-US':/^[A-Z\- ]+$/i,
	'cs-CZ':/^[A-ZÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ\- ]+$/i,
	'de-DE':/^[A-ZÄÖÜß\- ]+$/i,
	'es-ES':/^[A-ZÁÉÍÑÓÚÜ\- ]+$/i,
	'fr-FR':/^[A-ZÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ\- ]+$/i,
	'nl-NL':/^[A-ZÉËÏÓÖÜ\- ]+$/i,
	'hu-HU':/^[A-ZÁÉÍÓÖŐÚÜŰ\- ]+$/i,
	'pl-PL':/^[A-ZĄĆĘŚŁŃÓŻŹ\- ]+$/i,
	'pt-PT':/^[A-ZÃÁÀÂÇÉÊÍÕÓÔÚÜ\- ]+$/i,
	'ru-RU':/^[А-ЯЁа-яё\- ]+$/i,
	'tr-TR':/^[A-ZÇĞİıÖŞÜ\- ]+$/i,
	'he':/^[\u0590-\u05FF\- ]+$/,
	'iw':/^[\u0590-\u05FF\- ]+$/,
	'ar':/^[\u0600-\u06FF\- ]+$/,
	};

var alphanum={		//	Only 0-9 and a-Z  esp
	'en-US':/^[0-9A-Z ]+$/i,
	'cs-CZ':/^[0-9A-ZÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ ]+$/i,
	'de-DE':/^[0-9A-ZÄÖÜß ]+$/i,
	'es-ES':/^[0-9A-ZÁÉÍÑÓÚÜ ]+$/i,
	'fr-FR':/^[0-9A-ZÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ\- ]+$/i,
	'hu-HU':/^[0-9A-ZÁÉÍÓÖŐÚÜŰ ]+$/i,
	'nl-NL':/^[0-9A-ZÉËÏÓÖÜ ]+$/i,
	'pl-PL':/^[0-9A-ZĄĆĘŚŁŃÓŻŹ ]+$/i,
	'pt-PT':/^[0-9A-ZÃÁÀÂÇÉÊÍÕÓÔÚÜ ]+$/i,
	'ru-RU':/^[0-9А-ЯЁа-яё ]+$/i,
	'tr-TR':/^[0-9A-ZÇĞİıÖŞÜ ]+$/i,
	'he':/^[0-9\u0590-\u05FF ]+$/,
	'iw':/^[0-9\u0590-\u05FF ]+$/,
	'ar':/^[0-9\u0600-\u06FF ]+$/i,
	};

var alphafull={		//	for data base  0-9 and a-Z  and  -.,()/ esp £$€
	'en-US':/^[0-9A-Z\-\' .,()/£$€]+$/i,
	'cs-CZ':/^[0-9A-ZÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ\-\' .,()/£$€]+$/i,
	'de-DE':/^[0-9A-ZÄÖÜß\-\' .,()/£$€]+$/i,
	'es-ES':/^[0-9A-ZÁÉÍÑÓÚÜ\-\' .,()/£$€]+$/i,
	'fr-FR':/^[0-9A-ZÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ\-\' .,()/£$€]+$/i,
	'hu-HU':/^[0-9A-ZÁÉÍÓÖŐÚÜŰ\-\' .,()/£$€]+$/i,
	'nl-NL':/^[0-9A-ZÉËÏÓÖÜ\-\' .,()/£$€]+$/i,
	'pl-PL':/^[0-9A-ZĄĆĘŚŁŃÓŻŹ\-\' .,()/£$€]+$/i,
	'pt-PT':/^[0-9A-ZÃÁÀÂÇÉÊÍÕÓÔÚÜ\- .,()/£$€]+$/i,
	'ru-RU':/^[0-9А-ЯЁа-яё\-\' .,()/£$€]+$/i,
	'tr-TR':/^[0-9A-ZÇĞİıÖŞÜ\-\' .,()/£$€]+$/i,
	'he':/^[0-9\u0590-\u05FF\-\' .,()/£$€]+$/,
	'iw':/^[0-9\u0590-\u05FF\-\' .,()/£$€]+$/,
	'ar':/^[0-9\u0600-\u06FF\-\'  .,()/£$€]+$/i,
	};

//'

var englishLocales = ['AU','GB','HK','IN','NZ','ZA','ZM'];

for(let locale,i=0;i < englishLocales.length;i++){
	locale=`en-${englishLocales[i]}`;
	alpha[locale]=alpha['en-US'];
	alphanum[locale]=alphanum['en-US'];
	alphaname[locale]=alphaname['en-US'];
	alphafull[locale]=alphafull['en-US'];
	};

var arabicLocales=['AE','BH','DZ','EG','IQ','JO','KW','LB','LY','MA','QM','QA','SA','SD','SY','TN','YE',];

for(let locale,i=0;i<arabicLocales.length;i++){
	locale=`ar-${arabicLocales[i]}`;
	alpha[locale]=alpha['ar'];
	alphanum[locale]=alphanum['ar'];
	alphaname[locale]=alphaname['ar'];
	alphafull[locale]=alphafull['ar'];
	};

function isAlpha(str,locale='fr-FR'){
	if(locale in alpha){
		return alpha[locale].test(str);}
	else{ExceptionAdmin('isAlpha',locale);return false;};
};

function isAlphaName(str,locale='fr-FR'){
	if(locale in alphaname){
		return alphaname[locale].test(str);}
	else{ExceptionAdmin('isAlphaName',locale);return false;};
	};

function isAlphaNum(str,locale='fr-FR'){
	if(locale in alphanum){
		return alphanum[locale].test(str);}
	else{ExceptionAdmin('isAlphaNum',locale);return false;};
};

function isAlphaFull(str,locale='fr-FR'){
	if(locale in alphafull){
		return alphafull[locale].test(str);}
	else{ExceptionAdmin('isAlphaFull',locale);return false;};
};

var fmtdate={
	'yyyymmdd':/^\d{4}\d{2}\d{2}$/,
	'yyyy/mm/dd':/^\d{4}[/]\d{2}[/]\d{2}$/,
	'yyyy-mm-dd':/^\d{4}[-]\d{2}[-]\d{2}$/,
	'dd/mm/yyyy':/^\d{2}[/]\d{2}[/]\d{4}$/,
	'dd-mm-yyyy':/^\d{2}[-]\d{2}[-]\d{4}$/,
	'mm/dd/yyyy':/^\d{2}[/]\d{2}[/]\d{4}$/,
	'mm-dd-yyyy':/^\d{2}[-]\d{2}[-]\d{4}$/
	};

function isDateFmt(str,options){
	if(typeof(options)==='object'){
		var locale=options.format;}
	else{ExceptionAdmin('isDateFmt',locale);return false;};

	var _err=false;

	jQuery.each(fmtdate,function(idx){
		if(idx==locale){
			if(fmtdate[locale].test(str))_err=true;}});

	if(str.length!=locale.length||_err==false)return false;

	if(locale=='dd/mm/yyyy'||locale=='dd-mm-yyyy'){
		var day=str.substr(0,2);
		var month=str.substr(3,2);
		var year=str.substr(6,4);
	}

	else if(locale=='mm/dd/yyyy'||locale=='mm-dd-yyyy'){
		var month=str.substr(0,2);
		var day=str.substr(3,2);
		var year=str.substr(6,4);
	}

	else if(locale=='yyyymmdd'){
		var year=str.substr(0,4);
		var month=str.substr(4,2);
		var day=str.substr(6,2);
	}

	else if(locale=='yyyy/mm/dd'||locale=='yyyy-mm-dd'){
		var year=str.substr(0,4);
		var month=str.substr(7,2);
		var day=str.substr(8,2);
	}

	if((month<1)||(month>12))	return false;
	else if( (day<1) || (day>31) )	return false;
	else if( (month==2) && (day>28) )	return false;
	else if( ( (month==4) || (month==6) || (month==9) || (month==11) ) && (day>30) )	return false;
	else if( (month==2) && ( ( (year%400)==0 ) || ( (year%4) == 0) ) && ( (year%100) !=0 ) && (day>29) )	return false;
	else if( (month==2) && ( (year%100)==0 ) && (day>29) )return false;

	return true;
};


var digit=/^[0-9]+$/;
function isDigit(str){
	return digit.test(str);
};

var int=/^(?:[-+]?(?:0|[1-9][0-9]*))$/;
var intLeadingZeroes=/^[-+]?[0-9]+$/;

function isInt(str,options){
	 options=options||{};
	let regex=(options.hasOwnProperty('allow_leading_zeroes')&&options.allow_leading_zeroes?intLeadingZeroes:int);
	let minCheckPassed=(!options.hasOwnProperty('min')||str>=options.min);
	let maxCheckPassed=(!options.hasOwnProperty('max')||str<=options.max);
	return regex.test(str)&&minCheckPassed&&maxCheckPassed;
};



function isLength(str,options){
	let min;
	let max;
	if(typeof(options)==='object'){
		min=options.min||0;
		max=options.max;
		};
	const surrogatePairs=str.match(/[\uD800-\uDBFF][\uDC00-\uDFFF]/g)||[];
	const len=str.length-surrogatePairs.length;
	return len>=min&&(typeof max==='undefined'||len<=max);
};


function isPattern(str,options) {
	let vmatch = options;
	let pattern = new RegExp(vmatch,'i');
  return  pattern.test(str);
};

function isMail(str,options) {
	// From https://html.spec.whatwg.org/multipage/forms.html#valid-e-mail-address
 	// Retrieved 2014-01-14
    let vmatch =/^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/;
	let pattern = new RegExp(vmatch);

  return  pattern.test(str);
};