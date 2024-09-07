/**
 * Author: Fernando C.V.
 * http://lists.suckless.org/dev/1104/7590.html
 */

/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg) {
  Arg shifted;

  // left circular shift
  if (arg->i > 0)
    shifted.ui = (selmon->tagset[selmon->seltags] << arg->i) |
                 (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));

  // right circular shift
  else
    shifted.ui = selmon->tagset[selmon->seltags] >> (-arg->i) |
                 selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);

  view(&shifted);
}
