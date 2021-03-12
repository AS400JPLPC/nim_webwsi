jQuery.noConflict();

(function( $ ) {
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// define validator
//

//------------------------------------------------------------
// créer notre classe appelée Validator
// créer notre constructeur appelé comme méthode constructeur
// qui est responsable de configurer nos données de base.
//------------------------------------------------------------


  Validator = {

    constructor: function(form, config) {
      this._elForm = form;
      this._elmfld = config.fields || {};
      this._rcdfld = config.rcdfmt || {};

    },

//---------------------------------------------------------
//  ajouter la méthode pour valider ce qui est responsable
//  de tester contre l'exigence de champ nécessaire
//---------------------------------------------------------

	CrtlElmFld: function(elm  ) {

		let elm_field = this._elmfld;
		let field_err = true;
 	//  declaration variable global

		let _lang = $('html').attr('lang');


		let el_ID = $(elm).attr('id');

		let field_val = jQuery.trim($(elm).val());    $(elm).value = field_val; // trim rigth & left


		let trt_field =elm_field[el_ID];
		let err_ID = "#" + trt_field.message.errID;

		$.each(trt_field,function(index){
			switch(index) {
				case 'required' 	:  if (field_val.length == 0 && trt_field.required == true )	{
																field_err = false; return false ; }; break;

				case 'uppercase' 	:  { document.getElementById(el_ID).value = field_val.toUpperCase();
																field_val = field_val.toUpperCase();}; break;

				case 'lowercase' 	:  { document.getElementById(el_ID).value = field_val.toLowerCase();
																field_val = field_val.toLowerCase();}; break;

				case 'isLength' 	:  if (! isLength(field_val,trt_field[index]) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isAlpha' 		:  if (! isAlpha(field_val,_lang))	{
																field_err = false;  return false; };break;

				case 'isAlphaName' :  if (! isAlphaName(field_val,_lang) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isAlphaNum' :  if (! isAlphaNum(field_val,_lang) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isAlphaFull':  if (! isAlphaFull(field_val,_lang) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isDigit' 		:  if (! isDigit(field_val) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isInt' 			:  if (! isInt(field_val,trt_field[index]) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isDateFmt'	:  if (! isDateFmt(field_val,trt_field[index]) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isPattern'	:  if (! isPattern(field_val,trt_field[index]) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isMail'		  :  if (! isMail(field_val) && field_val.length > 0)	{
																field_err = false;  return false; };break;

				case 'isSelect'		:  	let el_select = "#"+ el_ID;
															if( $(el_select).val() =='**' && trt_field.isSelect == true) {
																field_err = false;  return false; };break;
			};
		});



 	//alert(err_ID+ "----" + Trtfld.message.errMsg+ "  flag:" + fieldError  );
	if (field_err == false){
		jQuery(err_ID).text(trt_field.message.errMsg);
		document.getElementById(el_ID).setCustomValidity('err');
	} else
	{ jQuery(err_ID).text("");
		document.getElementById(el_ID).setCustomValidity('');
	};	// fielderror



	return field_err;

	},   // validate elm_field


//---------------------------------------------------------
//  ajouter la méthode pour valider  le format
//  intégralement
//---------------------------------------------------------
	CtrlSubmit: function(e) {

		let elm_field = this._elmfld;
		let valid_err = true;
		let qery_field;
      for ( var  _elm in elm_field ) {
				qery_field = "#" + _elm;
				el_field = document.querySelector( qery_field);
				if( ! this.CrtlElmFld(el_field) )  valid_err= false;
			};
        e.preventDefault();
        return valid_err;
	},   // validate submit

//---------------------------------------------------------
//  ajouter la méthode pour clear
//  intégralement du format
//---------------------------------------------------------


	ClearTooltip: function(elm  ) {
		let _elmfield = this._elmfld;

		let _elID = $(elm).attr('id');
		let _Trt_fld = _elmfield[_elID];
		let _errID = "#" + _Trt_fld.message.errID;
		jQuery(_errID).text("");
	},

	ClearRcdFmt: function() {

      let rcd_field = this._rcdfld;
        jQuery.each(rcd_field ,function(el_ID){
				document.getElementById(el_ID).value ="";
				if(jQuery(rcd_field ).attr(el_ID).type == 'input' ) document.getElementById(el_ID).setCustomValidity('');
				vid_field = "#" + el_ID;
				el_field = document.querySelector( vid_field);
				form01.ClearTooltip(el_field);
				});


	} // clear input output


 };	//  validator



$(":input:first").focus();





//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// hors fonction key
$(':input').keyup(function (event) {

			// Avoid revalidate the field when pressing one of the following keys
			// Shift       => 16
			// Ctrl        => 17
			// Alt         => 18
			// Caps lock   => 20
			// End         => 35
			// Home        => 36
			// Left arrow  => 37
			// Up arrow    => 38
			// Right arrow => 39
			// Down arrow  => 40
			// Insert      => 45
			// Num lock    => 144
			// AltGr key   => 225
			var excludedKeys = [
				16, 17, 18, 20, 35, 36, 37,
				38, 39, 40, 45, 144, 225
			];

			if (  $.inArray( event.keyCode, excludedKeys ) !== 1 ) {
				return;
      }
			event.preventDefault();
			return;

});


$(':input').keypress(function (event) {

	if ($(this).attr('type') != 'submit' && $(this).attr('type') != 'reset') {

		var key = event.which;
 		if(key == 13  )  // the enter key code
		{
			$(this).trigger('change');
			return false;
		};

	};

});

$('input:text').last().on('keydown', function (e) {

	if (e.keyCode == 9) {
			$(":input:first").focus();
			e.preventDefault();
			return false;
	}
});


// hors fonction key
$('textarea').keyup(function (event) {

	// Avoid revalidate the field when pressing one of the following keys
	// Shift       => 16
	// Ctrl        => 17
	// Alt         => 18
	// Caps lock   => 20
	// End         => 35
	// Home        => 36
	// Left arrow  => 37
	// Up arrow    => 38
	// Right arrow => 39
	// Down arrow  => 40
	// Insert      => 45
	// Num lock    => 144
	// AltGr key   => 225
	var excludedKeys = [
		16, 17, 18, 20, 35, 36, 37,
		38, 39, 40, 45, 144, 225
	];

	if (  $.inArray( event.keyCode, excludedKeys ) !== 1 ) {
		return;
	}
	event.preventDefault();
	return;

});


$('textarea').keypress(function (event) {

	if ($(this).attr('type') != 'submit' && $(this).attr('type') != 'reset') {

		var key = event.which;
 		if(key == 13  )  // the enter key code
		{
			$(this).trigger('change');
			return false;
		};

	};

});

$('textarea').last().on('keydown', function (e) {

	if (e.keyCode == 9) {
			$(":input:first").focus();
			e.preventDefault();
			return false;
	}
});

/*
window.addEventListener("beforeunload", function (event) {
console.log("mgs:fin");
  event.returnValue = "\o/";
});
*/
})(jQuery);

