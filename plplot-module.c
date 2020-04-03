#include <emacs-module.h>

int plugin_is_GPL_compatible;

#include <plplot/plplot.h>

static char *
extract_utf8_string (emacs_env *env, emacs_value lisp_str)
{
  ptrdiff_t size = 0;
  char * buf = NULL;

  env->copy_string_contents (env, lisp_str, buf, &size);
  buf = malloc (size);
  env->copy_string_contents (env, lisp_str, buf, &size);
  return buf;
}

static double *
extract_vector_doubles (emacs_env *env, emacs_value vec)
{
  ptrdiff_t size = env->vec_size (env, vec);
  double *p = malloc (size * sizeof (double));
  for (ptrdiff_t i = 0; i < size; i++)
    p[i] = env->extract_float (env, env->vec_get (env, vec, i));
  return p;
}

static void
plfbox (PLFLT x0, PLFLT y0)
{
    PLFLT x[4], y[4];

    x[0] = x0;
    y[0] = 0.;
    x[1] = x0;
    y[1] = y0;
    x[2] = x0 + 1.;
    y[2] = y0;
    x[3] = x0 + 1.;
    y[3] = 0.;
    plfill (4, x, y);
    plcol0 (1);
    pllsty (1);
    plline (4, x, y);
}

static emacs_value
Fplot_bar_chart (emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data)
{
  double *xs, *ys;
  double xmin, xmax, ymin, ymax;
  char *xlabel, *ylabel, *title, *outfile;

  PLFLT pos[]   = {0.0, 0.25, 0.5, 0.75, 1.0};
  PLFLT red[]   = {0.0, 0.25, 0.5, 1.0, 1.0};
  PLFLT green[] = {1.0, 0.5, 0.5, 0.5, 1.0};
  PLFLT blue[]  = {1.0, 1.0, 0.5, 0.25, 0.0};

  xs = extract_vector_doubles (env, args[0]); /* not used */
  ys = extract_vector_doubles (env, args[1]);
  xmin = env->extract_float (env, args[2]);
  xmax = env->extract_float (env, args[3]);
  ymin = env->extract_float (env, args[4]);
  ymax = env->extract_float (env, args[5]);
  xlabel = extract_utf8_string (env, args[6]);
  ylabel = extract_utf8_string (env, args[7]);
  title = extract_utf8_string (env, args[8]);
  outfile = extract_utf8_string (env, args[9]);  

  plsdev ("svg");
  plsetopt ("o", outfile);
  plinit ();
  plenv (xmin, xmax, ymin, ymax, 0, 0);
  pllab (xlabel, ylabel, title);

  plscmap1l (1, 5, pos, red, green, blue, NULL);

  for (int i = 0; i < env->vec_size (env, args[0]); i++)
    {
      plcol1 (i / 9.0);
      plpsty (0);
      plfbox (i, ys[i]);
    }

  plend ();
  free (ys);
  free (xlabel);
  free (ylabel);
  free (title);
  free (outfile);
  return env->intern (env, "nil");
}

static emacs_value
Fplot (emacs_env *env, ptrdiff_t nargs, emacs_value *args, void *data)
{
  double *xs, *ys;
  double xmin, xmax, ymin, ymax;
  char *xlabel, *ylabel, *title, *outfile;

  xs = extract_vector_doubles (env, args[0]);
  ys = extract_vector_doubles (env, args[1]);
  xmin = env->extract_float (env, args[2]);
  xmax = env->extract_float (env, args[3]);
  ymin = env->extract_float (env, args[4]);
  ymax = env->extract_float (env, args[5]);
  xlabel = extract_utf8_string (env, args[6]);
  ylabel = extract_utf8_string (env, args[7]);
  title = extract_utf8_string (env, args[8]);
  outfile = extract_utf8_string (env, args[9]);

  plsdev ("svg");
  plsetopt ("o", outfile);
  plinit ();
  plenv (xmin, xmax, ymin, ymax, 0, 0);
  pllab (xlabel, ylabel, title);
  plline (env->vec_size (env, args[0]), xs, ys);
  plend ();

  free (xs);
  free (ys);
  free (xlabel);
  free (ylabel);
  free (title);
  free (outfile);
  
  return env->intern (env, "nil");
}


/* Lisp utilities for easier readability (simple wrappers).  */

/* Provide FEATURE to Emacs.  */
static void
provide (emacs_env *env, const char *feature)
{
  emacs_value Qfeat = env->intern (env, feature);
  emacs_value Qprovide = env->intern (env, "provide");
  emacs_value args[] = { Qfeat };

  env->funcall (env, Qprovide, 1, args);
}

/* Bind NAME to FUN.  */
static void
bind_function (emacs_env *env, const char *name, emacs_value Sfun)
{
  emacs_value Qdefalias = env->intern (env, "defalias");
  emacs_value Qsym = env->intern (env, name);
  emacs_value args[] = { Qsym, Sfun };

  env->funcall (env, Qdefalias, 2, args);
}

/* Module init function.  */
int
emacs_module_init (struct emacs_runtime *runtime)
{
  emacs_env *env = runtime->get_environment (runtime);

#define DEFUN(lsym, csym, amin, amax, doc, data)                        \
  bind_function (env, lsym,                                             \
		 env->make_function (env, amin, amax, csym, doc, data))

  DEFUN ("plplot-module-plot", Fplot, 10, 10, NULL, NULL);
  DEFUN ("plplot-module-plot-bar-chart", Fplot_bar_chart, 10, 10, NULL, NULL);
#undef DEFUN

  provide (env, "plplot-module");
  return 0;
}
