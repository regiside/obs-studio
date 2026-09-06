#include <obs-module.h>

static const char *tabs_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Property Tabs (Test)";
}

static void *tabs_create(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	UNUSED_PARAMETER(source);
	return bzalloc(1);
}

static void tabs_destroy(void *data)
{
	bfree(data);
}

static bool tabs_modified(obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	UNUSED_PARAMETER(property);
	obs_property_set_visible(obs_properties_get(props, "audio_tab"), !obs_data_get_bool(settings, "hide_audio"));
	obs_property_set_enabled(obs_properties_get(props, "audio_tab"), !obs_data_get_bool(settings, "disable_audio"));
	return true;
}

static obs_properties_t *tabs_properties(void *data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t *props = obs_properties_create();
	obs_property_t *property = obs_properties_add_bool(props, "hide_audio", "Hide audio tab");
	obs_property_set_modified_callback(property, tabs_modified);
	property = obs_properties_add_bool(props, "disable_audio", "Disable audio tab");
	obs_property_set_modified_callback(property, tabs_modified);

	obs_properties_t *video = obs_properties_create();
	obs_properties_add_text(video, "title", "Title", OBS_TEXT_DEFAULT);
	obs_properties_add_group(props, "video_tab", "Video", OBS_GROUP_TAB, video);

	obs_properties_t *audio = obs_properties_create();
	obs_properties_add_int_slider(audio, "volume", "Volume", 0, 100, 1);
	property = obs_properties_add_bool(audio, "refresh", "Refresh properties (keep this tab selected)");
	obs_property_set_modified_callback(property, tabs_modified);
	obs_properties_add_group(props, "audio_tab", "Audio", OBS_GROUP_TAB, audio);

	obs_properties_t *advanced = obs_properties_create();
	obs_properties_t *first = obs_properties_create();
	obs_properties_add_bool(first, "nested_toggle", "Nested toggle");
	obs_properties_add_group(advanced, "first_tab", "First", OBS_GROUP_TAB, first);
	obs_properties_t *second = obs_properties_create();
	obs_properties_add_text(second, "nested_text", "Nested text", OBS_TEXT_DEFAULT);
	obs_properties_add_group(advanced, "second_tab", "Second", OBS_GROUP_TAB, second);
	obs_properties_add_group(props, "advanced", "Advanced", OBS_GROUP_CHECKABLE, advanced);
	return props;
}

struct obs_source_info test_property_tabs = {
	.id = "test_property_tabs",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_AUDIO,
	.get_name = tabs_get_name,
	.create = tabs_create,
	.destroy = tabs_destroy,
	.get_properties = tabs_properties,
};
