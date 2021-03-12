
var fmtdate = {
        'yyyymmdd'	: /^\d{4}\d{2}\d{2}$/,
        'yyyy/mm/dd': /^\d{4}[./-]\d{2}[./-]\d{2}$/,         
        'yyyy-mm-dd': /^\d{4}[./-]\d{2}[./-]\d{2}$/, 
        'yyyy.mm.dd': /^\d{4}[./-]\d{2}[./-]\d{2}$/,               
        'dd/mm/yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/,
        'dd-mm-yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/,
        'dd.mm.yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/,
        'mm/dd/yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/,
        'mm-dd-yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/,
        'mm.dd.yyyy': /^\d{2}[./-]\d{2}[./-]\d{4}$/        
      };  
      
     
	
function isDateFmt(str) {
         var locale = arguments.length <= 1 || arguments[1] === undefined ? 'yyyymmdd' : arguments[1];

        assertString(str);
        if (locale in fmtdate) if( ! fmtdate[locale].test(str)) return false;


 
 
	    if( locale == 'dd/mm/yyyy' || locale == 'dd-mm-yyyy' || locale == 'dd.mm.yyyy') {   
       	 		var day   = str.substr(0,2);
		 		var month = str.substr(3,2);
		 		var year  = str.substr(6,4);
		        					};
 
	    if( locale == 'mm/dd/yyyy' || locale == 'mm-dd-yyyy' || locale == 'mm-dd-yyyy') {   
		 		var month = str.substr(0,2);
       	 		var day   = str.substr(3,2);
		 		var year  = str.substr(6,4);
		        					};		        
 
	    if( locale == 'yyyymmdd') {
		 		var year  = str.substr(0,4);   
		 		var month = str.substr(4,2);
       	 		var day   = str.substr(6,2);
		        					};		 
	    if( locale == 'yyyy/mm/dd' || locale == 'yyyy-mm-dd' || locale == 'yyyy-mm-dd') {
		 		var year  = str.substr(0,4);   
		 		var month = str.substr(7,2);
       	 		var day   = str.substr(8,2);
		        					};  
         
 	     
        if((month < 1) || (month > 12)) return false;      
        else if((day < 1) || (day > 31)) return false;
        else if((month == 2) && (day > 28)) return false;
        else if(((month == 4)  || (month == 6) || (month == 9)  || (month == 11)) && (day > 30)) return false;
		else if((month == 2) && (((year % 400) == 0) || ((year % 4) == 0)) && ((year % 100) != 0) && (day > 29)) return false;
        else if((month == 2) && ((year % 100) == 0) && (day > 29)) return false;
        

    	return true ; 
       
}
