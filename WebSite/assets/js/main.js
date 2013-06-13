$(document).ready(function(){

var url = 'ajax.php';
var update_url = 'http://ec2-23-23-188-178.compute-1.amazonaws.com:9001/update';

// Tweet moderation -- aprove or deny
$('.approve').click(function(){
	var id = $(this).parents('tr').attr('id');
	$.get(url, { id: id, mode: 'moderate', action: 'approve' }, function(data) {
		$('tr[id="'+data+'"]').children().css('background-color', 'lightgreen').delay(500).slideUp('fast');
	});
});
$('.deny').click(function(){
	var id = $(this).parents('tr').attr('id');
	$.get(url, { id: id, mode: 'moderate', action: 'deny' }, function(data) {
		$('tr[id="'+data+'"]').children().css('background-color', 'lightcoral').delay(500).slideUp('fast');
	});
});

// Tweet moderation -- set a category
$('.category-select li').click(function() {
	var id = $(this).parents('tr').attr('id');
	var category = $(this).attr('data-category');
	var categorySpan = $(this).parent().siblings().children('.current-category');

	$.get(url, { id: id, mode: 'category', category: category }, function(data) {
		categorySpan.html(data);
	});
});

// Whitelist moderation -- add or remove users
$('form#whitelist-add').submit(function() {
	var handle = $('form#whitelist-add input').val();
	handle = handle.trim();
	if (handle.substring(0, 1) == '@') {
		handle = handle.substring(1);
	}

	$.get(url, { mode: 'whitelist', action: 'add', handle: handle }, function(data) {
		if (data) {
			console.log(data);
			var append = '<tr id="'+data['_id']['$id']+'"><td><a class="btn btn-small whitelist-remove"><i class="icon-remove"></i></a></td><td><a href="http://twitter.com/'+data['user_screen_name']+'" target="_blank">@'+data['user_screen_name']+'</a></td><td>'+data['user_name']+'</td></tr>';
			$('table').append(append);

			$('form input').val('').css('background-color', 'white');

			$.get(update_url);
			//, function(){}).always(function() {
				//var alert_html = '<div class="alert alert-success"><button type="button" class="close" data-dismiss="alert">&times;</button>Stream has been restarted.</div>';
				//$('#alert_container').html(alert_html);
				// setTimeout(function(){
				// 	$('.alert').slideUp('fast', function(){$('#alert_container').html('')})
				// }, 2000);
			//});
		} else {
			$('form input').css('background-color', 'lightcoral');
		}
	}, 'json');

	return false;
});
$('.whitelist-remove').live('click', function() {
	var id = $(this).parents('tr').attr('id');
	$.get(url, { id: id, mode: 'whitelist', action: 'remove' }, function(data) {
		$('tr[id="'+data+'"]').children().css('background-color', 'lightcoral').delay(500).slideUp('fast');
		$.get(update_url);
	});
});

// Following moderation -- add or remove users
$('form#following-add').submit(function() {
	var handle = $('form#following-add input').val();
	handle = handle.trim();
	if (handle.substring(0, 1) == '@') {
		handle = handle.substring(1);
	}

	$.get(url, { mode: 'following', action: 'add', handle: handle }, function(data) {
		if (data) {
			console.log(data);
			var append = '<tr id="'+data['_id']['$id']+'"><td><a class="btn btn-small whitelist-remove"><i class="icon-remove"></i></a></td><td><a href="http://twitter.com/'+data['user_screen_name']+'" target="_blank">@'+data['user_screen_name']+'</a></td><td>'+data['user_name']+'</td></tr>';
			$('table').append(append);

			$('form input').val('').css('background-color', 'white');

			$.get(update_url);
		} else {
			$('form input').css('background-color', 'lightcoral');
		}
	}, 'json');

	return false;
});
$('.following-remove').live('click', function() {
	var id = $(this).parents('tr').attr('id');
	$.get(url, { id: id, mode: 'following', action: 'remove' }, function(data) {
		$('tr[id="'+data+'"]').children().css('background-color', 'lightcoral').delay(500).slideUp('fast');
		$.get(update_url);
	});
});

});