#include "EngineSettings.h"


EngineSettings::EngineSettings()
{
	//FILE *file;
	//yaml_parser_t parser;
	//yaml_document_t document;
	//int done = 0;
	//int count = 0;
	//int error = 0;

	////printf("[%d] Loading '%s': ", number, argv[number]);
	////fflush(stdout);

	//file = fopen("../res/engine.settings", "rb");
	//assert(file);

	//assert(yaml_parser_initialize(&parser));

	//yaml_parser_set_input_file(&parser, file);

	//while (!done)
	//{
	//	if (!yaml_parser_load(&parser, &document)) {
	//		error = 1;
	//		break;
	//	}

	//	done = (!yaml_document_get_root_node(&document));

	//	yaml_document_delete(&document);

	//	if (!done) count++;
	//}

	//yaml_parser_delete(&parser);

	//assert(!fclose(file));

	//printf("%s (%d documents)\n", (error ? "FAILURE" : "SUCCESS"), count);

	FILE *fh = fopen("../res/engine.settings", "r");
	yaml_parser_t parser;
	yaml_event_t  event;   /* New variable */

						   /* Initialize parser */
	if (!yaml_parser_initialize(&parser))
		fputs("Failed to initialize parser!\n", stderr);
	if (fh == NULL)
		fputs("Failed to open file!\n", stderr);

	/* Set input file */
	yaml_parser_set_input_file(&parser, fh);

	/* START new code */
	do {
		if (!yaml_parser_parse(&parser, &event)) {
			printf("Parser error %d\n", parser.error);
			exit(EXIT_FAILURE);
		}
		switch (event.type)
		{
		case YAML_NO_EVENT:
			puts("No event!");
			break;
			/* Stream start/end */
		case YAML_STREAM_START_EVENT:
			puts("STREAM START");
			break;
		case YAML_STREAM_END_EVENT:
			puts("STREAM END");
			break;
			/* Block delimeters */
		case YAML_DOCUMENT_START_EVENT:
			puts("<b>Start Document</b>");
			break;
		case YAML_DOCUMENT_END_EVENT:
			puts("<b>End Document</b>");
			break;
		case YAML_SEQUENCE_START_EVENT:
			puts("<b>Start Sequence</b>");
			break;
		case YAML_SEQUENCE_END_EVENT:
			puts("<b>End Sequence</b>");
			break;
		case YAML_MAPPING_START_EVENT:
			puts("<b>Start Mapping</b>");
			break;
		case YAML_MAPPING_END_EVENT:
			puts("<b>End Mapping</b>");
			break;
			/* Data */
		case YAML_ALIAS_EVENT:
			printf("Got alias (anchor %s)\n", event.data.alias.anchor);
			break;
		case YAML_SCALAR_EVENT:
			//printf("Got scalar (value %s)\n", event.data.scalar.value);
			onScalarRead(event);
			break;
		}
		if (event.type != YAML_STREAM_END_EVENT)
			yaml_event_delete(&event);
	} while (event.type != YAML_STREAM_END_EVENT);
	yaml_event_delete(&event);
	/* END new code */

	/* Cleanup */
	yaml_parser_delete(&parser);
	fclose(fh);
}


EngineSettings::~EngineSettings()
{
}

void EngineSettings::onScalarRead(yaml_event_t event)
{
	printf("!Got scalar (value %s)\n", event.data.scalar.value);
}