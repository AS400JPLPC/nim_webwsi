//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//
// traitement du formulaire
//
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


var form01 = Object.create(Validator);

form01.constructor('#formid', {

	fields: {
		'DATE': {
		required	: true,

		isLength:	{	min:10

					},
		isDateFmt:	{	format:'dd/mm/yyyy'

					},

		message:	{	errID: 'errDATE' ,
						errMsg:'le format de votre date est invalide'
					}
		},
		'NOM': {
		required	: true,

		uppercase	: true,

		isLength:	{
						max:30
					},
		isAlphaName:	true,

		message:	{	errID: 'errNOM' ,
						errMsg:'Votre nom est Obligatoire'
					}
		},
		'PRENOM': {
		required	: true,

		isLength:	{
						min:3,
						max:30
					},
		isAlphaName:	true, // alhpa + -

		message:	{	errID: 'errPRENOM' ,
						errMsg:'Votre prénom est Obligatoire'
					}
		},
		'NRUE': {
		required	: true,

		isLength:	{
						min:1,
						max:5
					},
		isDigit:	true,

		isInt:		{
						min:1,
						max:999
					},

		message:	{	errID: 'errNRUE' ,
						errMsg:'Votre numéro est invalide 1-999'
					}
		},
		'RUE1': {
		required	: true,

		isLength:	{
						min:0,
						max:30
					},
		isPattern	: "([0-9a-zÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ. \'\-])$" ,

		message:	{	errID: 'errRUE1' ,
						errMsg:'Votre nom de rue est incorrect.'
					}
		},
		'RUE2': {
		required	: false,

		isLength:	{
						min:0,
						max:30
					},
		isPattern	:	 "([0-9a-zÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ. \'\-])$"  ,

		message:	{	errID: 'errRUE2' ,
						errMsg:'la suite du nom de rue est incorrect.'
					}
		},
		'VILLE': {
		required	: true,


		isLength:	{
						min:0,
						max:30
					},
		isPattern	:	 "([0-9a-zÀÂÆÇÉÈÊËÏÎÔŒÙÛÜŸ. \'\-])$" ,

		message:	{	errID: 'errVILLE' ,
						errMsg:'la Ville est obligatoire.'
					}
		},
		'CPAYS': {

		isSelect	: true,

		message:	{	errID: 'errCPAY' ,
						errMsg:'le pays est obligatoire.'
					}
		}
	},
	rcdfmt: {		// win-> affichage  srv-> envoie  buffer -> serveur
			'DATE'	:	{ win:true, srv:true ,type:'input'},
			'NOM'	  :	{ win:true, srv:true ,type:'input'},
			'PRENOM':	{ win:true, srv:true ,type:'input'},
			'NRUE'	:	{ win:true, srv:true ,type:'input'},
			'RUE1'	:	{ win:true, srv:true ,type:'input'},
			'RUE2'	:	{ win:true, srv:true ,type:'input'},
			'VILLE'	:	{ win:true, srv:true ,type:'input'},
			'CPAYS'	:	{ win:true, srv:true ,type:'input'},
			'PAYS'	:	{ win:true, srv:false ,type:'text'},
			'FOOTER':	{ win:true, srv:false ,type:'msgsys'}
	}


});
