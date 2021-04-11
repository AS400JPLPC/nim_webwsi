/*
 * MIT License
 * Nim bindings for zserge's Webview which is an excellent cross-platform single header webview library for C/C++
 * using GTK
 * Copyright (c) 2017 Serge Zaitsev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef WEBWSI_H
#define WEBWSI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WEBWSI_STATIC
#define WEBWSI_API static
#else
#define WEBWSI_API extern
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(WEBWSI_GTK)
#include <JavaScriptCore/JavaScript.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <X11/Xlib.h>
#include <glib.h>

struct webwsi_priv {
  GtkWidget *window;
  GtkWidget *scroller;
  GtkWidget *webwin;
  GtkWidget *inspector_window;
  GAsyncQueue *queue;
  GtkWidget * btoggle;
  int ready;
  int js_busy;
  int should_exit;
};



#else
#error "Define one of: WEBWSI_GTK"
#endif


/// defined not optional
#define VTEFONT	"Monospace Regular"
#define MESSAGE_ALT_F4 "Confirmer la destruction de l'application  \n Confirm destroy Application"

struct webwsi;


typedef void (*webwsi_external_invoke_cb_t)(struct webwsi *w,
                                             const char *arg);

struct webwsi {
  const char *url;
  const char *title;
  int width;
  int height;
  int resizable;
  int fullscreen;
  int debug;
  bool toucheAltf4;
  bool buttonClose;
  bool taskbar;
  int  repDialog;

  int  mtitle;
  int  murl;
  int  mwidth;
  int  mheight;
  int  mfullscreen;


  webwsi_external_invoke_cb_t external_invoke_cb;
  struct webwsi_priv priv;

  struct webwsi_priv modl; /*modale */
  void *userdata;
};


#define webwsi_DIALOG_FLAG_INFO (1 << 1)
#define webwsi_DIALOG_FLAG_WARNING (2 << 1)
#define webwsi_DIALOG_FLAG_ERROR (3 << 1)
#define webwsi_DIALOG_FLAG_ALERT_MASK (3 << 1)

typedef void (*webwsi_dispatch_fn)(struct webwsi *w, void *arg);

struct webwsi_dispatch_arg {
  webwsi_dispatch_fn fn;
  struct webwsi *w;
  void *arg;
};

struct DialogInterne {
  const char *Title;
  const char *Message;
  int Flags;
};

static struct DialogInterne commsg;

//nombre de windows ouvert
static int  nbrwin;

#define DEFAULT_URL                                                            \
  "data:text/"                                                                 \
  "%3cmeta%20http-equiv%3d%22Content-Type%22%20content%3d%22text/html;%20charset%3dUTF-8%20%2f%3e"



#define CSS_INJECT_FUNCTION                                                    \
  "(function(e){var "                                                          \
  "t=document.createElement('style'),d=document.head||document."               \
  "getElementsByTagName('head')[0];t.setAttribute('type','text/"               \
  "css'),t.styleSheet?t.styleSheet.cssText=e:t.appendChild(document."          \
  "createTextNode(e)),d.appendChild(t)})"






static const char *webwsi_check_url(const char *url) {
  if (url == NULL || strlen(url) == 0) {
    return DEFAULT_URL;
  }
  return url;
}


WEBWSI_API void webwsi_terminate(struct webwsi *w);
///-------------------------------------
/// traitement ALT+F4
///-------------------------------------
WEBWSI_API gboolean key_press_ALTF4(WebKitWebView *webview,
                                    WebKitLoadEvent event, gpointer arg)
{

  struct webwsi *x = (struct webwsi *)arg;
	if ( x->toucheAltf4 == true)
	{

		GtkWidget *dialog;
		const gchar* _MSG_ =  MESSAGE_ALT_F4;

		dialog = gtk_message_dialog_new(
                    GTK_WINDOW(x->priv.window),
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_YES_NO,
                    _MSG_,
                    NULL,
                    NULL);

		int result = gtk_dialog_run (GTK_DIALOG (dialog));

		gtk_widget_destroy(GTK_WIDGET(dialog));

		switch (result)
		{
			case  GTK_RESPONSE_YES:
									{
                    webwsi_terminate(x);
                    x->url = "";
										return EXIT_FAILURE ;
										//break;
									}
			case GTK_RESPONSE_NO:
									// not active ALT_F4
									return GDK_EVENT_STOP;
									//break;
		}
	}
	// not active ALT_F4
	return GDK_EVENT_STOP;
}



//WEBWSI_API int webview(const char *title, const char *url, int width,
//                        int height, int resizable);


WEBWSI_API int webwsi_eval(struct webwsi *w, const char *js);
WEBWSI_API int webwsi_inject_css(struct webwsi *w, const char *css);




WEBWSI_API void webwsi_set_altf4(struct webwsi *w, int sflag){
  w->toucheAltf4 = sflag;
}

// utiliser apres hide ( modal / dialog )
WEBWSI_API void webwsi_show(struct webwsi *w,struct webwsi *r){
  if (nbrwin >= 1 ) {
    gtk_widget_show_all(w->priv.window);
    gtk_window_activate_default(w->priv.window);
  }
  nbrwin -= 1;
  if (nbrwin < 0 )  nbrwin = 0;
  gtk_widget_hide(r->priv.window);

}



// initialiser le premier ecran
WEBWSI_API void webwsi_show_init(struct webwsi *w){
  gtk_widget_show_all(w->priv.window);
  gtk_window_activate_default(w->priv.window);
}

/*
static void hide_modal(struct webwsi *w ){
  w->url = webwsi_check_url("");
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin), w->url);
  gtk_window_set_modal(w->priv.window,FALSE);
  gtk_window_set_keep_above(w->priv.window,FALSE);

}
*/

WEBWSI_API void webwsi_hide_modal(struct webwsi *w){
  gtk_widget_hide(w->priv.window);
  //g_idle_add(hide_modal,w);
  //g_source_remove(idle_id);
}



static void webwsi_show_modal(struct webwsi *w){
  gtk_window_set_modal(w->priv.window,TRUE);
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin), w->url);
  gtk_widget_show_all(w->priv.window);
  gtk_window_set_keep_above(w->priv.window,TRUE);
}


WEBWSI_API void webwsi_modal(struct webwsi *w , const char * url){
  w->url = url;
  nbrwin += 1;
  gtk_window_resize(GTK_WINDOW(w->priv.window), w->width, w->height);
  g_idle_add(webwsi_show_modal,w );
}

/*
static void hide_dialogx(struct webwsi *w ){
  w->url = webwsi_check_url("");
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin), w->url);
  gtk_window_set_keep_above(w->priv.window,FALSE);
}
*/

WEBWSI_API void webwsi_hide_dialogx(struct webwsi *w){
  gtk_widget_hide(w->priv.window);
}



static void webwsi_show_dialogx(struct webwsi *w){
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin), w->url);
  gtk_widget_show_all(w->priv.window);
  gtk_window_set_keep_above(w->priv.window,TRUE);

}

WEBWSI_API void webwsi_dialogx(struct webwsi *w , const char * url){
  w->url = url;
  nbrwin += 1;
  gtk_window_resize(GTK_WINDOW(w->priv.window), w->width, w->height);
  g_idle_add(webwsi_show_dialogx,w );
}




WEBWSI_API void webwsi_destroy(struct webwsi *w){
  gtk_widget_destroy(GTK_WINDOW(w->priv.window));
}

WEBWSI_API void webwsi_set_title(struct webwsi *w, const char *title) {
  gtk_window_set_title(GTK_WINDOW(w->priv.window), title);
}

WEBWSI_API void webwsi_set_fullscreen(struct webwsi *w) {
  if (w->fullscreen  == FALSE) {
    gtk_window_fullscreen(GTK_WINDOW(w->priv.window));
    w->fullscreen  = TRUE;
  } else {
    gtk_window_unfullscreen(GTK_WINDOW(w->priv.window));
    w->fullscreen = FALSE;
  }
}


WEBWSI_API void webwsi_setDeletable(struct webwsi *w, int sflag){
  w->buttonClose = sflag;
  if (w->buttonClose == false )  gtk_window_set_deletable (GTK_WINDOW(w->priv.window), FALSE);
  else gtk_window_set_deletable (GTK_WINDOW(w->priv.window), TRUE);

}



WEBWSI_API void webwsi_setresizable(struct webwsi *w, int sflag){
  w->resizable= sflag;
  gtk_window_set_resizable(GTK_WINDOW(w->priv.window), !!w->resizable);

}




WEBWSI_API void webwsi_setTaskBar(struct webwsi *w, int sflag){
  w->taskbar= sflag;
  gtk_window_set_skip_taskbar_hint(GTK_WINDOW(w->priv.window), w->taskbar);
  gtk_window_set_skip_pager_hint(GTK_WINDOW(w->priv.window), w->taskbar);


}

WEBWSI_API void webwsi_change_url(struct webwsi *w, const char *url) {
  w->url = url;
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin), w->url);
}
/*
  GTK_RESPONSE_NONE         = -1,
  GTK_RESPONSE_REJECT       = -2,
  GTK_RESPONSE_ACCEPT       = -3,
  GTK_RESPONSE_DELETE_EVENT = -4,
  GTK_RESPONSE_OK           = -5,
  GTK_RESPONSE_CANCEL       = -6,
  GTK_RESPONSE_CLOSE        = -7,
  GTK_RESPONSE_YES          = -8,
  GTK_RESPONSE_NO           = -9,
  GTK_RESPONSE_APPLY        = -10,
  GTK_RESPONSE_HELP         = -11
*/

static gboolean  webwsi_dialog_msg( struct webwsi *w)
{


  GtkWidget *dialog;
  GtkMessageType msgType ;
  GtkButtonsType buttonType ;
  GtkWidget *WIN = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  const int m_info = 0;
  const int m_warn = 1;
  const int m_error = 2 ;
  const int m_query = 3;
  int value = 0;
  if (commsg.Flags == m_info) { msgType = GTK_MESSAGE_INFO ;     buttonType = GTK_BUTTONS_OK; }
  if (commsg.Flags == m_warn) { msgType = GTK_MESSAGE_WARNING ;  buttonType = GTK_BUTTONS_OK; }
  if (commsg.Flags == m_error) { msgType = GTK_MESSAGE_ERROR ;    buttonType = GTK_BUTTONS_OK; }
  if (commsg.Flags == m_query) { msgType = GTK_MESSAGE_QUESTION ; buttonType = GTK_BUTTONS_YES_NO; }

  dialog = gtk_message_dialog_new(
                    WIN,
                    GTK_DIALOG_MODAL,
                    msgType,
                    buttonType,
                    commsg.Title);

  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", commsg.Message);

  int result = gtk_dialog_run (GTK_DIALOG (dialog));

  gtk_widget_destroy(GTK_WIDGET(dialog));

  switch (msgType) {
    case  GTK_MESSAGE_QUESTION : {
      switch (result)
      {
        case  GTK_RESPONSE_YES: w->repDialog = GTK_RESPONSE_YES ;  return false ;
        case  GTK_RESPONSE_NO:  w->repDialog =  GTK_RESPONSE_NO ;  return false ;
        default :    w->repDialog =  GTK_RESPONSE_REJECT ;  return false ;
      };
    }
    default : {
      switch (result)
      {
        case  GTK_RESPONSE_OK: w->repDialog = GTK_RESPONSE_OK ;  return false ;
        default :    w->repDialog =  GTK_RESPONSE_REJECT ;  return false ;
      };
    }
  };
  w->repDialog = GTK_RESPONSE_REJECT ;
  return false ;
};

WEBWSI_API void webwsi_setMessage(struct webwsi *w, int * sflag,const char * title, const char * message){
  w->repDialog = 999;
  commsg.Flags = sflag;
  commsg.Title = title;
  commsg.Message = message;
  g_idle_add(webwsi_dialog_msg,w);

}


WEBWSI_API void webwsi_set_color(struct webwsi *w, uint8_t r, uint8_t g,
                                    uint8_t b, uint8_t a);

WEBWSI_API void webwsi_dispatch(struct webwsi *w, webwsi_dispatch_fn fn,
                                  void *arg);

WEBWSI_API void webwsi_exit(struct webwsi *w);
WEBWSI_API void webwsi_debug(const char *format, ...);
WEBWSI_API void webwsi_print_log(const char *s);

#ifdef WEBWSI_IMPLEMENTATION
#undef WEBWSI_IMPLEMENTATION



WEBWSI_API void webwsi_debug(const char *format, ...) {
  char buf[4096];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  webwsi_print_log(buf);
  va_end(ap);
}

static int webwsi_js_encode(const char *s, char *esc, size_t n) {
  int r = 1; /* At least one byte for trailing zero */
  for (; *s; s++) {
    const unsigned char c = *s;
    if (c >= 0x20 && c < 0x80 && strchr("<>\\'\"", c) == NULL) {
      if (n > 0) {
        *esc++ = c;
        n--;
      }
      r++;
    } else {
      if (n > 0) {
        snprintf(esc, n, "\\x%02x", (int)c);
        esc += 4;
        n -= 4;
      }
      r += 4;
    }
  }
  return r;
}

WEBWSI_API int webwsi_inject_css(struct webwsi *w, const char *css) {
  int n = webwsi_js_encode(css, NULL, 0);
  char *esc = (char *)calloc(1, sizeof(CSS_INJECT_FUNCTION) + n + 4);
  if (esc == NULL) {
    return -1;
  }
  char *js = (char *)calloc(1, n);
  webwsi_js_encode(css, js, n);
  snprintf(esc, sizeof(CSS_INJECT_FUNCTION) + n + 4, "%s(\"%s\")",
            CSS_INJECT_FUNCTION, js);
  int r = webwsi_eval(w, esc);
  free(js);
  free(esc);
  return r;
}
WEBWSI_API void webwsi_set_font(struct webwsi *w,  const char *css) {
    webwsi_inject_css(w,(const char*)css);
}

#if defined(WEBWSI_GTK)
static void external_message_received_cb(WebKitUserContentManager *m,
                                          WebKitJavascriptResult *r,
                                          gpointer arg) {
  (void)m;
  struct webwsi *w = (struct webwsi *)arg;
  if (w->external_invoke_cb == NULL) {
    return;
  }
  JSGlobalContextRef context = webkit_javascript_result_get_global_context(r);
  JSValueRef value = webkit_javascript_result_get_value(r);
  JSStringRef js = JSValueToStringCopy(context, value, NULL);
  size_t n = JSStringGetMaximumUTF8CStringSize(js);
  char *s = g_new(char, n);
  JSStringGetUTF8CString(js, s, n);
  w->external_invoke_cb(w, s);
  JSStringRelease(js);
  g_free(s);
}

static void webwsi_load_changed_cb(WebKitWebView *webview,
                                    WebKitLoadEvent event, gpointer arg) {
  (void)webview;
  struct webwsi *w = (struct webwsi *)arg;
  if (event == WEBKIT_LOAD_FINISHED) {
    w->priv.ready = 1;
  }
}

static void webwsi_destroy_cb(GtkWidget *widget, gpointer arg) {
  (void)widget;
  struct webwsi *w = (struct webwsi *)arg;
  webwsi_terminate(w);
}





static gboolean webwsi_context_menu_cb(WebKitWebView *webview,
                                        GtkWidget *default_menu,
                                        WebKitHitTestResult *hit_test_result,
                                        gboolean triggered_with_keyboard,
                                        gpointer userdata) {
  (void)webview;
  (void)default_menu;
  (void)hit_test_result;
  (void)triggered_with_keyboard;
  (void)userdata;
  return TRUE;
}



WEBWSI_API int webwsi_init(struct webwsi *w ) {
  if (gtk_init_check(0, NULL) == FALSE) {
    return -1;
  }


  if (w->width == 0 ||  w->height == 0) {
    GdkScreen*  scr = gdk_screen_get_default ();
    gint width   = gdk_screen_get_width(scr);
    gint height  = gdk_screen_get_height(scr);
    if ( width <= 1920  ) { w->width = width - 100 ; w->height = height - 100 ; }
    else if ( width > 1920 ) { w->width = 1920  ; w->height = 1280  ; }
  }


  w->priv.ready = 0;
  w->priv.should_exit = 0;
  w->priv.queue = g_async_queue_new();
  w->priv.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(w->priv.window), w->title);
  //gtk_container_set_border_width(GTK_CONTAINER(w->priv.window),10);
  if (w->resizable) {
    gtk_window_set_default_size(GTK_WINDOW(w->priv.window), w->width, w->height);
  } else {
    gtk_widget_set_size_request(w->priv.window, w->width, w->height);
  }
  gtk_window_set_resizable(GTK_WINDOW(w->priv.window), !!w->resizable);
  gtk_window_set_position(GTK_WINDOW(w->priv.window), GTK_WIN_POS_CENTER);


  w->priv.scroller = gtk_scrolled_window_new(NULL, NULL);
  //gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(w->priv.scroller), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(w->priv.window), w->priv.scroller);



  WebKitUserContentManager *m = webkit_user_content_manager_new();
  webkit_user_content_manager_register_script_message_handler(m, "external");
  g_signal_connect(m, "script-message-received::external",
                    G_CALLBACK(external_message_received_cb), w);

  w->priv.webwin = webkit_web_view_new_with_user_content_manager(m);
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webwin),
                            webwsi_check_url(w->url));
  g_signal_connect(G_OBJECT(w->priv.webwin), "load-changed",
                    G_CALLBACK(webwsi_load_changed_cb), w);
  gtk_container_add(GTK_CONTAINER(w->priv.scroller), w->priv.webwin);

  g_signal_connect(G_OBJECT(w->priv.window) ,"delete_event",
                    G_CALLBACK(key_press_ALTF4), w);

  if (w->debug) {
    WebKitSettings *settings =
        webkit_web_view_get_settings(WEBKIT_WEB_VIEW(w->priv.webwin));
    webkit_settings_set_enable_write_console_messages_to_stdout(settings, true);
    webkit_settings_set_enable_developer_extras(settings, true);
  } else {
    g_signal_connect(G_OBJECT(w->priv.webwin), "context-menu",
                      G_CALLBACK(webwsi_context_menu_cb), w);
  }

  //webwsi_set_font(w,"style.type = 'text/css'; style.fontFamily ='monospace'; style.font-size: 2.5em;");
      /*font-size: 2.5em;   /*40px/16=2.5em */
      /*font-size: 1.875em;  30px/16=1.875em */


  if (w->url != "" ) gtk_widget_show_all(w->priv.window);
  else gtk_window_set_keep_above(w->priv.window,FALSE);

  webkit_web_view_run_javascript(
      WEBKIT_WEB_VIEW(w->priv.webwin),
      "window.external={invoke:function(x){"
      "window.webkit.messageHandlers.external.postMessage(x);}}",
      NULL, NULL, NULL);

  g_signal_connect(G_OBJECT(w->priv.window), "destroy",
                    G_CALLBACK(webwsi_destroy_cb), w);


  return 0;
}












WEBWSI_API int webwsi_loop(struct webwsi *w, int blocking) {
  gtk_main_iteration_do(blocking);
  return w->priv.should_exit;
}

WEBWSI_API void webwsi_set_color(struct webwsi *w, uint8_t r, uint8_t g,
                                    uint8_t b, uint8_t a) {
  GdkRGBA color = {r / 0.0, g / 0.0, b / 0.0, a / 0.0};
  webkit_web_view_set_background_color(WEBKIT_WEB_VIEW(w->priv.webwin),
                                        &color);
}

static void webwsi_eval_finished(GObject *object, GAsyncResult *result,
                                  gpointer userdata) {
  (void) object;
  (void) result;
  struct webwsi *w = (struct webwsi *)userdata;
  w->priv.js_busy = 0;
}

WEBWSI_API int webwsi_eval(struct webwsi *w, const char *js) {
  while (w->priv.ready == 0) {
    g_main_context_iteration(NULL, TRUE);
  }
  w->priv.js_busy = 1;
  webkit_web_view_run_javascript(WEBKIT_WEB_VIEW(w->priv.webwin), js, NULL,
                                  webwsi_eval_finished, w);
  while (w->priv.js_busy) {
    g_main_context_iteration(NULL, TRUE);
  }
  return 0;
}

static gboolean webwsi_dispatch_wrapper(gpointer userdata) {
  struct webwsi *w = (struct webwsi *)userdata;
  for (;;) {
    struct webwsi_dispatch_arg *arg =
        (struct webwsi_dispatch_arg *)g_async_queue_try_pop(w->priv.queue);
    if (arg == NULL) {
      break;
    }
    (arg->fn)(w, arg->arg);
    g_free(arg);
  }
  return FALSE;
}

WEBWSI_API void webwsi_dispatch(struct webwsi *w, webwsi_dispatch_fn fn,
                                  void *arg) {
  struct webwsi_dispatch_arg *context =
      (struct webwsi_dispatch_arg *)g_new(struct webwsi_dispatch_arg, 1);
  context->w = w;
  context->arg = arg;
  context->fn = fn;
  g_async_queue_lock(w->priv.queue);
  g_async_queue_push_unlocked(w->priv.queue, context);
  if (g_async_queue_length_unlocked(w->priv.queue) == 1) {
    gdk_threads_add_idle(webwsi_dispatch_wrapper, w);
  }
  g_async_queue_unlock(w->priv.queue);
}

WEBWSI_API void webwsi_terminate(struct webwsi *w) {
  w->priv.should_exit = 1;
}

WEBWSI_API void webwsi_exit(struct webwsi *w) { (void)w; }
WEBWSI_API void webwsi_print_log(const char *s) {
  fprintf(stderr, "%s\n", s);
}


/* $$$$$$$$$$$$$$$$$$ MODAL $$$$$$$$$$$$$$$$$$*/



WEBWSI_API void modale_close(struct webwsi *w){
      gtk_widget_destroy(GTK_WINDOW(w->modl.window));
}

WEBWSI_API void modal_set_fullscreen(struct webwsi *w) {
  if (w->mfullscreen  == FALSE) {
    gtk_window_fullscreen(GTK_WINDOW(w->priv.window));
    w->mfullscreen  = TRUE;
  } else {
    gtk_window_unfullscreen(GTK_WINDOW(w->priv.window));
    w->mfullscreen = FALSE;
  }
}

//-------------------------------------
/// traitement ALT+F4
///-------------------------------------
static gboolean modal_press_ALTF4(WebKitWebView *webview,
                                    WebKitLoadEvent event, gpointer arg)
{

  struct webwsi *x = (struct webwsi *)arg;
		GtkWidget *dialog;
		const gchar* _MSG_ =  MESSAGE_ALT_F4;

		dialog = gtk_message_dialog_new(
                    GTK_WINDOW(x->modl.window),
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_YES_NO,
                    _MSG_,
                    NULL,
                    NULL);

		int result = gtk_dialog_run (GTK_DIALOG (dialog));

		gtk_widget_destroy(GTK_WIDGET(dialog));

		switch (result)
		{
			case  GTK_RESPONSE_YES:
									{
									  modale_close(x);
										return EXIT_FAILURE ;
										//break;
									}
			case GTK_RESPONSE_NO:
									// not active ALT_F4
									return GDK_EVENT_STOP;
									//break;
		}
	// not active ALT_F4
	return GDK_EVENT_STOP;
}




static void modal_load_changed_cb(WebKitWebView *webview,
                                    WebKitLoadEvent event, gpointer arg) {
  (void)webview;
  struct webwsi *w = (struct webwsi *)arg;
  if (event == WEBKIT_LOAD_FINISHED) {
    w->modl.ready = 1;
  }
}


static gboolean modal_context_menu_cb(WebKitWebView *webview,
                                        GtkWidget *default_menu,
                                        WebKitHitTestResult *hit_test_result,
                                        gboolean triggered_with_keyboard,
                                        gpointer userdata) {
  (void)webview;
  (void)default_menu;
  (void)hit_test_result;
  (void)triggered_with_keyboard;
  (void)userdata;
  return TRUE;
}

static void modal_destroy_cb(GtkWidget *widget, gpointer arg) {
  // modal_close
  // moadl_press_ALTF4
}



static int modal_init(struct webwsi *w ) {
  if (gtk_init_check(0, NULL) == FALSE) {
    return -1;
  }


  if (w->mwidth == 0 ||  w->mheight == 0) {
    GdkScreen*  scr = gdk_screen_get_default ();
    gint width   = gdk_screen_get_width(scr);
    gint height  = gdk_screen_get_height(scr);
    if ( width <= 1920  ) { w->mwidth = width - 100 ; w->mheight = height - 100 ; }
    else if ( width > 1920 ) { w->mwidth = 1920  ; w->mheight = 1280  ; }
  }


  w->modl.ready = 0;
  w->modl.should_exit = 0;
  w->modl.queue = g_async_queue_new();
  w->modl.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(w->modl.window), w->mtitle);
  gtk_window_set_modal(GTK_WINDOW(w->modl.window), TRUE);
  //gtk_container_set_border_width(GTK_CONTAINER(w->modl.window),10);

  gtk_window_set_default_size(GTK_WINDOW(w->modl.window), w->mwidth, w->mheight);

  gtk_window_set_resizable(GTK_WINDOW(w->modl.window), TRUE);
  gtk_window_set_position(GTK_WINDOW(w->modl.window), GTK_WIN_POS_CENTER);


  w->modl.scroller = gtk_scrolled_window_new(NULL, NULL);
  //gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(w->modl.scroller), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(w->modl.window), w->modl.scroller);

  printf("799");

  WebKitUserContentManager *m = webkit_user_content_manager_new();
  webkit_user_content_manager_register_script_message_handler(m, "external");

  w->modl.webwin = webkit_web_view_new_with_user_content_manager(m);


  w->modl.webwin = w->priv.webwin;
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->modl.webwin),
                            webwsi_check_url(w->murl));
  /*g_signal_connect(G_OBJECT(w->modl.webwin), "load-changed",
                    G_CALLBACK(modal_load_changed_cb), w); */
  gtk_container_add(GTK_CONTAINER(w->modl.scroller), w->modl.webwin);

  g_signal_connect(G_OBJECT(w->modl.window) ,"delete_event",
                    G_CALLBACK(modal_press_ALTF4), w);

  if (w->debug) {
    WebKitSettings *settings =
        webkit_web_view_get_settings(WEBKIT_WEB_VIEW(w->modl.webwin));
    webkit_settings_set_enable_write_console_messages_to_stdout(settings, true);
    webkit_settings_set_enable_developer_extras(settings, true);
  } else {
    g_signal_connect(G_OBJECT(w->modl.webwin), "context-menu",
                      G_CALLBACK(modal_context_menu_cb), w);
  }

  //webwsi_set_font(w,"style.type = 'text/css'; style.fontFamily ='monospace'; style.font-size: 2.5em;");
      /*font-size: 2.5em;   /*40px/16=2.5em */
      /*font-size: 1.875em;  30px/16=1.875em */


  gtk_widget_show_all(w->modl.window);
  webkit_web_view_run_javascript(
      WEBKIT_WEB_VIEW(w->modl.webwin),
      "window.external={invoke:function(x){"
      "window.webkit.messageHandlers.external.postMessage(x);}}",
      NULL, NULL, NULL);

  g_signal_connect(G_OBJECT(w->modl.window), "destroy",
                    G_CALLBACK(modal_destroy_cb), w);

  return 0;
}



WEBWSI_API void modal_windows(struct webwsi *w , const char * mtitle, const char* murl, int* mwidth  , int* mheight  ) {
    w->mtitle = mtitle;
    w->murl = murl;
    w->mwidth = mwidth;
    w->mheight = mheight;
    g_idle_add(modal_init,w);

}






#endif /* WEBWSI_GTK */



#endif /* WEBWSI_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* WEBWSI_H */
