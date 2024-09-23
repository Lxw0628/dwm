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
  unsigned int newtagset = selmon->tagset[selmon->seltags];
  unsigned int occupiedtags = 0;
  unsigned int scratchtags = SPTAG(0) | SPTAG(1) | SPTAG(2);  // 假设有 3 个 scratchpad 标签

  // 检查每个标签是否有窗口，忽略 scratchtags
  Client *c;
  for (c = selmon->clients; c; c = c->next)
    occupiedtags |= c->tags & ~scratchtags;  // 过滤掉 scratchpad 标签

  // 左循环移位
  if (arg->i > 0) {
    do {
      newtagset = (newtagset << arg->i) | (newtagset >> (LENGTH(tags) - arg->i));
    } while (!(newtagset & occupiedtags));  // 跳过没有窗口的标签
  }
  // 右循环移位
  else {
    do {
      newtagset = (newtagset >> (-arg->i)) | (newtagset << (LENGTH(tags) + arg->i));
    } while (!(newtagset & occupiedtags));  // 跳过没有窗口的标签
  }

  shifted.ui = newtagset & ~scratchtags;  // 确保新标签集中不包含 scratchpad 标签
  view(&shifted);
}
