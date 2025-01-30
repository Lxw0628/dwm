void
tagtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		arrange(selmon);
		focus(NULL);
	}
}

void
tagtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		selmon->sel->tags <<= 1;
		arrange(selmon);
		focus(NULL);
	}
}

#if !VIEWTOTAGIGNOREEMPTY
void
viewtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] >> 1 }));
	}
}
void
viewtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] << 1 }));
	}
}
#else
// 新增辅助函数：检查某个标签是否有窗口
static int
taghasclient(int tag) {
    Client *c;
    for (c = selmon->clients; c; c = c->next) {
        if (c->tags & (1 << (tag - 1))) // 假设标签从 1 开始编号（dwm 默认）
            return 1;
    }
    return 0;
}
void
viewtoleft(const Arg *arg) {
    unsigned int curtag = selmon->tagset[selmon->seltags];
    int newtag = 0;
    // 向左切换（物理上的高位到低位）
    for (int i = 1; i < NUMTAGS; i++) {
        /* 循环右移 */  // [!code focus]
        newtag = (curtag >> 1) | ((curtag & 1) << (NUMTAGS - 1)); // [!code focus]
        if (newtag == curtag) break;
        if (newtag & TAGMASK && taghasclient(ffs(newtag))) {
            view(&(Arg){.ui = newtag});
            return;
        }
        curtag = newtag; // [!code focus]
    }
}
void
viewtoright(const Arg *arg) {
    unsigned int curtag = selmon->tagset[selmon->seltags];
    int newtag = 0;
    // 向右切换（物理上的低位到高位）
    for (int i = 1; i < NUMTAGS; i++) {
        /* 循环左移 */  // [!code focus]
        newtag = (curtag << 1) | ((curtag >> (NUMTAGS - 1)) & 1); // [!code focus]
        if (newtag == curtag) break;
        if (newtag & TAGMASK && taghasclient(ffs(newtag))) {
            view(&(Arg){.ui = newtag});
            return;
        }
        curtag = newtag; // [!code focus]
    }
}
#endif // VIEWTOTAGIGNOREEMPTY

void
tagandviewtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] >> 1 }));
	}
}

void
tagandviewtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		selmon->sel->tags <<= 1;
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] << 1 }));
	}
}
