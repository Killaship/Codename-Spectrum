
void sleep(unsigned int milliseconds) {
	__asm__ volatile("cli");

	int inseconds = milliseconds / 1000;
	int inminutes = inseconds / 60;
	int inhours = inminutes / 60;
	inminutes = inminutes % 60;
	inseconds = inseconds % 60;

	inseconds += getSeconds();
	inminutes += getMinutes();
	inhours += getHours();

	while (inseconds > 60) {
		inminutes += 1;
		inseconds -= 60;
	}

	while (inminutes > 60) {
		inhours += 1;
		inminutes -= 60;
	}

	while (getSeconds() <= inseconds && getMinutes() <= inminutes && getHours() <= inhours);
	
	__asm__ volatile("sti");
}
