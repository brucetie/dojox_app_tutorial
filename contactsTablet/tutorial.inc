<?php require("../../../resources/tutHead.inc"); ?>

<h3 id="contactsAppTabletAddDetails">The Views</h3>
<p>The previous version of the <strong>Contacts Application</strong> has support for two views, the
	<strong>list</strong> view and the <strong>details</strong> view.  In this tutorial we will add support to use a
	two column layout when the application is run on a tablet, or a desktop browser which is sized to be larger than a
	phone, when the application is loaded.</p>
<div style="float: left; width: 100%; margin-top: 20px;">
	<p>This is how the <strong>Contacts Application</strong> will look on an iPad iOS Simulator.</p>
	<div style="float: left; width: 96%;">
		<label style="margin-left: 70px;">Contacts Application on a tablet in "View" Mode</label>
		<img src="images/tabletImage.png" alt="Phone Contacts List View" style="margin:0 auto;display:block;" />
	</div>
</div>
<div style="float: left; width: 100%; margin-top: 20px; margin-bottom: 20px">
	<div style="float: left; width: 96%;">
		<label style="margin-left: 70px;">Contacts Application on a tablet in "Edit" Mode</label>
		<img src="images/tabletUpdate.png" alt="Phone Contacts Details View" style="margin:0 auto;display:block;" />
	</div>
</div>

<p>Since we are using dojox/app and we already have the <strong>Contacts Application</strong> running on a phone, it is easy to update the
application to have it use a two column layout on a tablet.  These updates are needed to use a two column layout:</p>
	<ul>
		<li>Update the CSS file for the application to use a media query so that the list will be shown on the left and the back button
			hidden when not running on a phone.</li>
		<li>Update the The JSON configuration file to use the "has" configuration support to set "constraint": "left" for the list
			if we are not on a phone.</li>
		<li>Update the main module of the application to set the "has" value for "phone" to true or false depending upon the
		size of the window or client.</li>
		<li>Update the "list" view handle the selection indicator on the list and to also show the "details" view initially
			when not on a phone.</li>
	</ul>


<h3 id="updatecss">The CSS file for the application.</h3>

<p>Update the <strong>application CSS</strong> (contacts.css) to include a media query which will take effect when not running on a phone:

	The CSS media query looks like this:</p>
<pre class="brush: js; html-script: true; highlight: [1,2,7]">
	@media screen and (min-width: 501px) {
	  	.leftpanel {
	    	width: 250px;
			border-right:1px solid black;
			z-index:100;
		}
		.backButton {
			display: none;
		}
	}
</pre>

<h3 id="updatejson">The JSON configuration file</h3>

<p>The JSON configuration file (contacts.json) is updated to add the following:</p>
<ul>
	<li>The <strong>"has"</strong> check for "!phone" used to set the constraint to "left" for the "list" view when not running on a phone.</li>
</ul>

<pre class="brush: js; html-script: true; highlight: [6,7,8,9,10]">
	"views": {
		"list": {
			"controller": "contactsAppTablet/views/list",
			"template": "contactsAppTablet/views/list.html",
			"nls": "contactsAppTablet/nls/list"
			"has": {
				"!phone": {
					"constraint": "left"
				}
			}
		},
</pre>

<h3 id="updatemain">The main module of the application</h3>
<p>The main module (contacts.js) is updated to define the <strong>has</strong> configuration flag for <strong>phone</strong>.</p>
<pre class="brush: js; html-script: true; highlight: [9,10]">
	// we use 'define' and not 'require' to workaround Dojo build system
	// limitation that prevents from making of this file a layer if it
	// using 'require'
	define(["dojo/sniff", "dojo/json", "dojox/mobile/common",
		"dojo/text!contactsAppTablet/contacts.json", "dojox/app/main"],
		function(has, json, common, config, Application){

		 has.add("html5history", !has("ie") || has("ie") > 9);
		 has.add("phone", ((window.innerWidth ||
			document.documentElement.clientWidth) <= common.tabletSize));
		 Application(json.parse(config));

	});
</pre>

<h3 id="updateviews">The <strong>"list"</strong> view</h3>
<p>The <strong>"list" view template</strong> (list.html) is updated to:</p>
<ul>
	<li>Add a class named "leftpanel" to the outer div on the list view template.</li>
	<li>Add a dojo/require for "dojo/has" in order to use "has" in the template.</li>
	<li>Use !has('phone') for stateful on the EdgeToEdgeStoreList so the selection will be shown when not running on a phone.</li>
</ul>

<pre class="brush: html; highlight: [1,3,20]">
	&lt;div class="leftpanel mblBackground"&gt;
		&lt;script type="dojo/require"&gt;
			has: "dojo/has"
		&lt;/script&gt;
		&lt;div data-dojo-type="dojox/mobile/Heading"
			 data-dojo-props="fixed: 'top'"&gt;
			${nls.contacts}
			&lt;button data-dojo-type="dojox/mobile/ToolBarButton"
					style="position: absolute; right: 0"
					data-dojo-props="target: 'details',
						transitionOptions: { params: { edit: true } }"
					data-dojo-attach-point="add"&gt;${nls.add}&lt;/button&gt;
		&lt;/div&gt;
		&lt;div data-dojo-type="dojox/mobile/EdgeToEdgeStoreList"
				data-dojo-mixins="dojox/mobile/FilteredListMixin"
				data-dojo-attach-point="contacts"
				data-dojo-props="store: this.loadedStores.contacts,
					labelProperty: 'displayName',
					itemRenderer: this.ContactListItem,
					placeHolder: '${nls.search}', stateful: !has('phone')"&gt;
		&lt;/div&gt;
	&lt;/div&gt;
</pre>


<p>The <strong>"list" view controller</strong> (list.js) is updated to:</p>
<ul>
	<li>Set <strong>noArrow: !has("phone"),</strong> on ContactListItem to only show the arrow on a phone.</li>
	<li>Add an <strong>init function</strong>.</li>
	<ul>
		<li>The <strong>init function</strong> sets up a listener which is called when an item is added to the contact list. This listener will set
			the "selectedItem" on the list to the item which was just added.</li>
		<li>The <strong>init function</strong> also sets up a listener which is called when the "Add" button is pressed, which will call
			"deselectAll" to clear the selection on the list.</li>
	</ul>
	<li>Add a <strong>beforeActivate function</strong>.</li>
	<ul>
		<li>The <strong>beforeActivate function</strong> will call the "app.transitionToView" function to transition to the "details" view
			when the application is initialized if not running on a phone.</li>
	</ul>
</ul>

<pre class="brush: js; html-script: true; highlight: [9,22,24,26,30,37,41,44,55]">
	define(["dojo/_base/declare", "dojo/_base/array", "dojo/has",
		"dojox/mobile/ListItem", "dojox/mobile/EdgeToEdgeStoreList",
		"dojox/mobile/FilteredListMixin"],
		function(declare, array, has, ListItem){
		var ContactListItem = declare(ListItem, {
			target: "details",
			clickable: true,
			// we don't get an arrow if we are on a two panes layout (tablet)
			noArrow: !has("phone"),
			postMixInProperties: function(){
				this.inherited(arguments);
				this.transitionOptions = {
					params: {
						"id" : this.id
					}
				}
			}
		});

		return {
			ContactListItem: ContactListItem,
			init: function(){
				var view = this;
				this.contacts.on("add", function(item){
					// select the newly added element
					if(!has("phone")){
						array.some(view.contacts.getChildren(),
							function(child){
								if(child.id == item.id){
									view.contacts.selectItem(child);
								}
								return false;
							}
						);
					}
				});
				this.add.on("click", function(){
					view.contacts.deselectAll();
				});
			},
			beforeActivate: function(){
				// in tablet we want one to be selected each time but list
				// must be completed for that
				if(!has("phone")){
					// check if something is selected
					var selected = array.some(this.contacts.getChildren(),
						function(child){
							return child.get("selected");
						}
					);
					if(!selected && this.contacts.getChildren().length > 0){
						var item = this.contacts.getChildren()[0];
						this.contacts.selectItem(item);
						// transition
						this.app.transitionToView(this.domNode, {
							target: "details",
							params: {
								id: item.id
							}
						});
					}
				}
			}
		};
	});
</pre>

<h3 id="updatedetailsviews">The <strong>"details"</strong> view</h3>

<p>The "details" view does not require any changes to move from a phone form factor to a tablet form factor.</p>

<a class="demoBtn demo" href="demos/contactsAppTablet/contacts.php" target="_blank">Run the Contacts Application</a>
<a class="demoBtn demo" href="https://github.com/edchat/dojox_app_tutorial/tree/master/contactsTablet/demos/contactsAppTablet" target="_blank">View the contactsAppTablet source on github</a>

<h3>Conclusion</h3>

<p>In this tutorial, we've learned how to update the Contacts Application from supporting only a phone form factor
	to add support for a tablet form factor in addition to the phone form factor.  The tablet form factor uses a 2
	column layout with the "list" view on the left and the "details" view in the center.
</p>

<?php include("../series.inc"); ?>

