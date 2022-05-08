const db_air = firebase.database();
const display_air=document.querySelector('#air');
const ref_air = db_air.ref('live_data/'+'air/');

ref_air.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_air.innerHTML = data;
    
})