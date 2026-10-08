#ifndef LANG_H
#define LANG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize language module ("id" or "en") */
void lang_init(const char *lang_code);

/* Set active language ("id" or "en") */
void lang_set(const char *lang_code);

/* Get current active language code */
const char *lang_get_current(void);

/* Retrieve localized string by key */
const char *lang_str(const char *key);

/* Shorthand macro */
#define _T(k) lang_str(k)

#ifdef __cplusplus
}
#endif

#endif /* LANG_H */
