#ifndef PICORBC_MISC_H_
#define PICORBC_MISC_H_

#include <mruby.h>

/* from proc.h */
void mrb_env_unshare(mrb_state*, struct REnv*);

/* from irep.h */
MRB_API mrb_value mrb_load_irep(mrb_state *mrb, const uint8_t *bin);

/* from dump.h */
MRB_API mrb_value mrb_load_irep_file_cxt(mrb_state*, FILE*, picorbc_context*);

MRB_API mrb_value
mrb_load_string(mrb_state *mrb, const char *str, picorbc_context *c)
{
  mrb_value ret;
  StreamInterface *si = StreamInterface_new(NULL, (char *)str, STREAM_TYPE_MEMORY);
  ParserState *p = Compiler_parseInitState(si->node_box_size);
  if (Compiler_compile(p, si, c)) {
    mrb_load_irep(mrb, p->scope->vm_code);
  } else {
  }
  return ret;
}

MRB_API mrb_value
mrb_load_string_cxt(mrb_state *mrb, const char *s, picorbc_context *c)
{
  //return mrb_load_nstring_cxt(mrb, s, strlen(s), c);
  return mrb_load_string(mrb, s, c);
}

/* from proc.h */
struct REnv {
  MRB_OBJECT_HEADER;
  mrb_value *stack;
  struct mrb_context *cxt;
  mrb_sym mid;
};

/* from proc.h */
static inline struct REnv *
mrb_vm_ci_env(const mrb_callinfo *ci)
{
  if (ci->u.env && ci->u.env->tt == MRB_TT_ENV) {
    return ci->u.env;
  }
  else {
    return NULL;
  }
}

/* from proc.h */
static inline void
mrb_vm_ci_env_set(mrb_callinfo *ci, struct REnv *e)
{
  if (ci->u.env) {
    if (ci->u.env->tt == MRB_TT_ENV) {
      if (e) {
        e->c = ci->u.env->c;
        ci->u.env = e;
      }
      else {
        ci->u.target_class = ci->u.env->c;
      }
    }
    else {
      if (e) {
        e->c = ci->u.target_class;
        ci->u.env = e;
      }
    }
  }
  else {
    ci->u.env = e;
  }
}

#endif /* PICORBC_MISC_H_*/
